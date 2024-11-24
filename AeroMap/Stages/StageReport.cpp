// StageReport.cpp
// Generate PDF report.
//

#include <fstream>
#include <nlohmann/json.h>
using json = nlohmann::json;

#include <QPdfWriter>

#include "StageReport.h"

const int OFFSETV = 240;					// default line spacing
const QColor TABLE_COLOR(218, 222, 228);	// table fill color
const int TABLE_MARGIN = 100;

int StageReport::Run()
{
	// Inputs:
	// Outputs:
	//		+ odm_report
	//			report.pdf
	//

	int status = 0;

	BenchmarkStart("Report");

	AeroLib::CreateFolder(tree.report_path);

	XString file_name = XString::CombinePath(tree.report_path, "report.pdf");
	QFile outputFile(file_name.c_str());
	if (!outputFile.open(QIODevice::WriteOnly))
	{
		Logger::Write(__FUNCTION__, "Unable to create '%s'", file_name.c_str());
		assert(false);
		return -1;
	}

	mp_Writer = new QPdfWriter(&outputFile);
	mp_Writer->setPageSize(QPageSize(QPageSize::Letter));
	mp_Writer->setPageOrientation(QPageLayout::Portrait);
	m_dpi = mp_Writer->resolution();
	m_marginl = m_dpi / 2;
	m_marginr = 0;

	mp_Painter = new QPainter(mp_Writer);

	SetupFonts();
	LoadOpenSfmStats();

	OutputSummary();
	OutputOpenSFM();
	OutputOrthophoto();
	OutputParameters();

	delete mp_Painter;

	outputFile.close();
	delete mp_Writer;

	BenchmarkStop();

	return status;
}

void StageReport::OutputSummary()
{
	OutputHeader(false);

	mp_Painter->setFont(m_FontHeader1);
	mp_Painter->drawText(m_marginl, 1000, "Summary");

	mp_Painter->setFont(m_FontBody);
	int y = 1400;

	// lines in table
	int line_count = 5;

	QRectF rectTable(m_marginl, y, m_dpi * 6, OFFSETV * line_count);
	QRectF rectLabel = rectTable;
	rectLabel.setWidth(rectTable.width() / 2);
	mp_Painter->fillRect(rectLabel, TABLE_COLOR);
	QPen pen(TABLE_COLOR, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	mp_Painter->setPen(pen);
	mp_Painter->drawRect(rectTable);
	for (int i = 1; i < line_count; ++i)
	{
		int line_y = rectTable.top() + OFFSETV * i;
		mp_Painter->drawLine(rectTable.left(), line_y, rectTable.right(), line_y);
	}
	mp_Painter->setPen(QColor(0, 0, 0));

	const int COL1 = rectTable.left() + TABLE_MARGIN;
	const int COL2 = rectLabel.right() + TABLE_MARGIN;
	y = rectTable.top();
	y += 180;
	OutputText(COL1, y, "Date Processed:"); OutputText(COL2, y, "%s", m_process_date.c_str()); y += OFFSETV;
	OutputText(COL1, y, "Date Collected:"); OutputText(COL2, y, "%s", m_collect_date.c_str()); y += OFFSETV;
	OutputText(COL1, y, "Area Covered:");   OutputText(COL2, y, "%0.7f sq km", m_area * 1E-6); y += OFFSETV;
	OutputText(COL1, y, "Images:");         OutputText(COL2, y, "%d", GetProject().GetImageCount()); y += OFFSETV;

	OutputText(COL1, y, "Location:");
	AeroLib::Georef georef = AeroLib::ReadGeoref();
	if (georef.is_valid)
	{
		double lat, lon;
		GIS::XYToLatLon_UTM(georef.utm_zone, georef.hemi, georef.x, georef.y, lat, lon, GIS::Ellipsoid::WGS_84);
		OutputText(COL2, y, "%0.3f %c %0.3f %c", lat, lat >= 0.0 ? 'N' : 'S', lon, lon >= 0.0 ? 'E' : 'W');
	}
	else
	{
		OutputText(COL2, y, "---");
	}
	y += OFFSETV;
}

void StageReport::OutputOpenSFM()
{
	OutputHeader();

	mp_Painter->setFont(m_FontHeader1);
	mp_Painter->drawText(m_marginl, 1000, "OpenSFM");

	// top view
	{
		XString file_name = XString::CombinePath(tree.opensfm, "stats/topview.png");
		if (AeroLib::FileExists(file_name) == false)
			return;

		QImage image;
		if (image.load(file_name.c_str()) == false)
		{
			Logger::Write(__FUNCTION__, "Unable to load: '%s'", file_name.c_str());
			return;
		}

		// desired size of image, inches
		const double WIDTH_IN = 6.5;
		double height_in = ((double)image.height() / (double)image.width()) * WIDTH_IN;

		QRect rectTarget(1000, 1600, m_dpi * WIDTH_IN, m_dpi * height_in);
		QRect rectSrc = image.rect();
		mp_Painter->drawImage(rectTarget, image, rectSrc, Qt::AutoColor);
	}
}

void StageReport::OutputOrthophoto()
{
	if (AeroLib::FileExists(tree.orthophoto_tif) == false)
		return;

	QImage image;
	if (image.load(tree.orthophoto_tif.c_str()) == false)
	{
		Logger::Write(__FUNCTION__, "Unable to load orthophoto: '%s'", tree.orthophoto_tif.c_str());
		return;
	}

	int MAX_IMAGE_SIZE = 800;
	int w = image.width();
	int h = image.height();
	int max_dim = (w > h ? w : h);
	if (max_dim > MAX_IMAGE_SIZE)
	{
		// since image is stored in pdf - and loaded every time
		// report is viewed - scale down to a reasonable size
		double sf = (double)MAX_IMAGE_SIZE / (double)max_dim;
		image = image.scaled(w * sf, h * sf, Qt::AspectRatioMode::KeepAspectRatio);
	}

	XString ortho_file = XString::CombinePath(tree.report_path, "ortho.png");
	if (image.save(ortho_file.c_str(), "PNG") == false)
	{
		Logger::Write(__FUNCTION__, "Unable to write orthophoto: '%s'", ortho_file.c_str());
		return;
	}

	OutputHeader();

	mp_Painter->setFont(m_FontHeader1);
	mp_Painter->drawText(m_marginl, 1000, "Orthophoto");

	// desired size of image, inches
	const double WIDTH_IN = 6.5;
	double height_in = ((double)image.height() / (double)image.width()) * WIDTH_IN;

	QRect rectTarget(1000, 1600, m_dpi * WIDTH_IN, m_dpi * height_in);
	QRect rectSrc = image.rect();
	mp_Painter->drawImage(rectTarget, image, rectSrc, Qt::AutoColor);
}

void StageReport::OutputParameters()
{
	OutputHeader();

	mp_Painter->setFont(m_FontHeader1);
	mp_Painter->drawText(m_marginl, 1000, "Parameters");

	mp_Painter->setFont(m_FontBody);
	int y = 1400;

	int line_count = 5;

	QRectF rectTable(m_marginl, y, m_dpi * 6, OFFSETV * line_count);
	QRectF rectLabel = rectTable;
	rectLabel.setWidth(rectTable.width() / 2);
	mp_Painter->fillRect(rectLabel, TABLE_COLOR);
	QPen pen(TABLE_COLOR, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	mp_Painter->setPen(pen);
	mp_Painter->drawRect(rectTable);
	for (int i = 1; i < line_count; ++i)
	{
		int line_y = rectTable.top() + OFFSETV * i;
		mp_Painter->drawLine(rectTable.left(), line_y, rectTable.right(), line_y);
	}
	mp_Painter->setPen(QColor(0, 0, 0));

	const int COL1 = rectTable.left() + TABLE_MARGIN;
	const int COL2 = rectLabel.right() + TABLE_MARGIN;
	y = rectTable.top();
	y += 180;

	OutputText(COL1, y, "DEM Resolution:"); OutputText(COL2, y, "%0.1f", arg.dem_resolution); y += OFFSETV;
	OutputText(COL1, y, "Orthophoto Resolution:"); OutputText(COL2, y, "%0.1f", arg.ortho_resolution); y += OFFSETV;
	OutputText(COL1, y, "DSM:"); OutputText(COL2, y, "%s", arg.dsm ? "True" : "False"); y += OFFSETV;
	OutputText(COL1, y, "DTM:"); OutputText(COL2, y, "%s", arg.dtm ? "True" : "False"); y += OFFSETV;
	OutputText(COL1, y, "Fast Orthophoto:"); OutputText(COL2, y, "%s", arg.fast_orthophoto ? "True" : "False"); y += OFFSETV;
}

void StageReport::OutputHeader(bool new_page)
{
	if (new_page)
		mp_Writer->newPage();

	mp_Painter->setFont(m_FontHeader);
	mp_Painter->drawText(m_marginl, 400, "AeroMap Report");
}

void StageReport::SetupFonts()
{
	m_FontHeader.setFamily("Cambria");
	m_FontHeader.setPointSize(12.0);
	m_FontHeader.setBold(true);
	m_FontHeader.setItalic(false);

	m_FontHeader1.setFamily("Cambria");
	m_FontHeader1.setPointSize(11.0);
	m_FontHeader1.setBold(true);
	m_FontHeader1.setItalic(false);

	m_FontBody.setFamily("Consolas");
	m_FontBody.setPointSize(9.0);
	m_FontBody.setBold(false);
	m_FontBody.setItalic(false);
}

void StageReport::LoadOpenSfmStats()
{
	m_collect_date.Clear();
	m_area = 0.0;

	XString file_name = XString::CombinePath(tree.opensfm, "stats/stats.json");
	if (AeroLib::FileExists(file_name) == false)
		return;

	std::ifstream f(file_name.c_str());
	json data = json::parse(f);
	
	json proc_stat = data["processing_statistics"];
	if (proc_stat != nullptr)
	{
		// looks convoluted, but this is how nlohman recommends 
		// explicitly casting from json types
		json js_run_date = proc_stat["date"];
		if (js_run_date != nullptr)
			m_process_date = js_run_date.template get<std::string>().c_str();
		json js_end_date = proc_stat["end_date"];
		if (js_end_date != nullptr)
			m_collect_date = js_end_date.template get<std::string>().c_str();

		m_area = proc_stat["area"];
	}
}

void StageReport::OutputText(int x, int y, const char* text, ...)
{
	char buf[255] = { 0 };

	va_list ap;
	va_start(ap, text);
	vsprintf(buf, text, ap);
	va_end(ap);

	mp_Painter->drawText(x, y, buf);
}

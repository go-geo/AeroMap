#ifndef STAGEREPORT_H
#define STAGEREPORT_H

#include "Stage.h"

class StageReport : Stage
{
public:

	virtual int Run() override;

private:

	QFont m_FontHeader;
	QFont m_FontHeader1;
	QFont m_FontBody;

	QPdfWriter* mp_Writer;
	QPainter* mp_Painter;

	int m_dpi;

	XString m_collect_date;
	XString m_process_date;
	double m_area;

private:

	void SetupFonts();
	void OutputSummary();
	void OutputOpenSFM();
	void OutputOrthophoto();
	void OutputParameters();
	void OutputHeader(bool new_page = true);
	void OutputText(int x, int y, const char* text, ...);

	void LoadOpenSfmStats();
};

#endif // #ifndef STAGEREPORT_H

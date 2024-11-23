// Stage.cpp
// Base class for all stages.
//

#include "Stage.h"

void Stage::BenchmarkStart(const char* name)
{
	assert(name != nullptr);

	// update ui
	GetApp()->LogWrite(XString::Format("%s...", name).c_str());

	// write current stage to run log to help with debugging
	AeroLib::WriteRunLog("========================================");
	AeroLib::WriteRunLog(XString::Format("Stage: %s", name).c_str());
	AeroLib::WriteRunLog("========================================");

	m_Timer.start();
}

void Stage::BenchmarkStop(bool init)
{
	double sec = m_Timer.elapsed() * 0.001;

	XString file_name = XString::CombinePath(GetProject().GetDroneOutputPath(), "benchmark.txt");

	FILE* pFile = nullptr;
	if (init)
		pFile = fopen(file_name.c_str(), "wt");
	else
		pFile = fopen(file_name.c_str(), "at");

	if (pFile)
	{
		fprintf(pFile, "%s: %0.3f\n", m_StageName.c_str(), sec);
		fclose(pFile);
	}
}

bool Stage::Rerun()
{
	// Force stage to run regardless of output
	// file state.
	//

//TODO:
//
	return true;
}
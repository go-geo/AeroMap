#ifndef STAGEDEM_H
#define STAGEDEM_H

#include "Stage.h"

class StageDEM : Stage
{
public:

	virtual int Run() override;

private:

	void generate_terrain_model(XString product, double resolution);
};

#endif // #ifndef STAGEDEM_H

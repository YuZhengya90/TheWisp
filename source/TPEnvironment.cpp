#include "TPEnvironment.h"

TPPoint TPPoint::operator - (const TPPoint& p)
{
	return TPPoint(x - p.x, y - p.y);
}

TPPoint TPPoint::operator + (const TPPoint& p)
{
	return TPPoint(x + p.x, y + p.y);
}

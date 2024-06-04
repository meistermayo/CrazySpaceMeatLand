#include "Quat.h"

const Quat Quat::Identity(Vect::Zero);

Quat operator * (float scale, Quat& inQ)
{
	return inQ * scale;
}

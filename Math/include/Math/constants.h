#pragma once

#ifdef _COMPLEX_ADDED
#define I Complex(0.f,1.f)
#endif // _COMPLEX_ADDED

#ifdef QUATERNION_ADDED
#define Iq Quaternion(0.f,1.f,0.f,0.f)
#define Jq Quaternion(0.f,0.f,1.f,0.f)
#define Kq Quaternion(0.f,0.f,0.f,1.f)
#endif // QUATERNION_ADDED

#define pi 3.14159265358979f
#define en 2.71828182845905f
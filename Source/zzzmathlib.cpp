// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#include "stdafx.h"
#include "zzzmathlib.h"

//GS-N 0.99.60T 0x0048A960
//	GS-n	1.00.18	JPN	0x004A5AD0

//-----------------------------------------------------------------------------
// mathlib.cpp -- math primitives



//vec3_t vec3_origin = {0.0f,0.0f,0.0f};

float VectorLength(vec3_t v)	// Good
{
	int		i;
	float	length;
	
	length = 0.0f;
	for (i=0 ; i< 3 ; i++)
		length += v[i]*v[i];
	length = sqrtf (length);

	return length;
}

qboolean VectorCompare (vec3_t v1, vec3_t v2) // Good
{
	int		i;
	
	for (i=0 ; i<3 ; i++)
		if (fabs(v1[i]-v2[i]) > EQUAL_EPSILON)
			return false;
			
	return true;
}

vec_t Q_rint (vec_t in)	// Good
{
	return floor(in + 0.5);
}

void VectorMA (vec3_t va, float scale, vec3_t vb, vec3_t vc)	// Good
{
	vc[0] = va[0] + scale*vb[0];
	vc[1] = va[1] + scale*vb[1];
	vc[2] = va[2] + scale*vb[2];
}

void CrossProduct (vec3_t v1, vec3_t v2, vec3_t cross) // Good
{
	cross[0] = v1[1]*v2[2] - v1[2]*v2[1];
	cross[1] = v1[2]*v2[0] - v1[0]*v2[2];
	cross[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

vec_t _DotProduct (vec3_t v1, vec3_t v2) // Good
{
	return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

void _VectorSubtract (vec3_t va, vec3_t vb, vec3_t out) // Good
{
	out[0] = va[0]-vb[0];
	out[1] = va[1]-vb[1];
	out[2] = va[2]-vb[2];
}

void _VectorAdd (vec3_t va, vec3_t vb, vec3_t out) // Good
{
	out[0] = va[0]+vb[0];
	out[1] = va[1]+vb[1];
	out[2] = va[2]+vb[2];
}

void _VectorCopy (vec3_t in, vec3_t out) // Good
{
	out[0] = in[0];
	out[1] = in[1];
	out[2] = in[2];
}


void VectorScale (vec3_t v, vec_t scale, vec3_t out) // Good
{
	out[0] = v[0] * scale;
	out[1] = v[1] * scale;
	out[2] = v[2] * scale;
}


vec_t VectorNormalize (vec3_t v) // Good
{
	int		i;
	float	length;
	
	if ( fabs(v[1]-0.0002159560000) < 0.0001 )
	{
		i=1;
	}
	length=0;
	for (i=0 ; i< 3 ; i++)
		length += v[i]*v[i];
	length = sqrtf (length);
	if (length == 0)
		return (vec_t)0;
		
	for (i=0 ; i< 3 ; i++)
		v[i] /= length;	

	return length;
}

void VectorInverse (vec3_t v) // Good
{
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
}

void ClearBounds (vec3_t mins, vec3_t maxs) // Good
{
	mins[0] = mins[1] = mins[2] = 99999;
	maxs[0] = maxs[1] = maxs[2] = -99999;
}



void AddPointToBounds (vec3_t v, vec3_t mins, vec3_t maxs) // Good
{
	int		i;
	vec_t	val;

	for (i=0 ; i<3 ; i++)
	{
		val = v[i];
		if (val < mins[i])
			mins[i] = val;
		if (val > maxs[i])
			maxs[i] = val;
	}
}



#define	PITCH				0		// up / down
#define	YAW					1		// left / right
#define	ROLL				2		// fall over
#ifndef M_PI
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
#endif

/*void AngleVectors (vec3_t angles, vec3_t forward, vec3_t right, vec3_t up)
{
	float		angle;
	float		sr, sp, sy, cr, cp, cy;
	// static to help MS compiler fp bugs

	angle = (float)( angles[YAW] * (M_PI*2 / 360) );
	sy = (float)sin(angle);
	cy = (float)cos(angle);
	angle = (float)( angles[PITCH] * (M_PI*2 / 360) );
	sp = (float)sin(angle);
	cp = (float)cos(angle);
	angle = (float)( angles[ROLL] * (M_PI*2 / 360) );
	sr = (float)sin(angle);
	cr = (float)cos(angle);

	if (forward)
	{
		forward[0] = cp*cy;
		forward[1] = cp*sy;
		forward[2] = -sp;
	}
	if (right)
	{
		right[0] = -sr*sp*cy+cr*sy;
		right[1] = -sr*sp*sy-cr*cy;
		right[2] = -sr*cp;
	}
	if (up)
	{
		up[0] = cr*sp*cy+sr*sy;
		up[1] = cr*sp*sy-sr*cy;
		up[2] = cr*cp;
	}
}*/
void AngleMatrix (const float *angles, float  matrix [3][4] )	// Good - Strange this0.0174532925199432955f
{
	float		angle;
	float		sr, sp, sy, cr, cp, cy;
	
	angle = angles[ROLL] *	0.0174532925199432955f;		//0.0174532821447436905f;
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[YAW] *   0.0174532925199432955f;
	sp = sin(angle);   //ST=0.0174532923847436905
	cp = cos(angle);
	angle = angles[PITCH] * 0.0174532925199432955f;		//0.0174532821247436905f;
	sr = sin(angle);
	cr = cos(angle);

	// matrix = (YAW * PITCH) * ROLL
	matrix[0][0] = cp*cy;
	matrix[1][0] = cp*sy;
	matrix[2][0] = -sp;
	matrix[0][1] = sr*sp*cy+cr*-sy;
	matrix[1][1] = sr*sp*sy+cr*cy;
	matrix[2][1] = sr*cp;
	matrix[0][2] = (cr*sp*cy+-sr*-sy);
	matrix[1][2] = (cr*sp*sy+-sr*cy);
	matrix[2][2] = cr*cp;
	matrix[0][3] = 0.0;
	matrix[1][3] = 0.0;
	matrix[2][3] = 0.0;
}

void AngleIMatrix (const vec3_t angles, float matrix[3][4] ) // GoodStrange this 0.0174532925199432955f
{
	float angle;
	float sr, sp, sy, cr, cp, cy;
	
	angle = angles[ROLL] * (float)(0.0174532925199432955f);
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[YAW] * (float)(0.0174532925199432955f);
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[PITCH] * (float)(0.0174532925199432955f);	// M_PI*2 /360
	sr = sin(angle);
	cr = cos(angle);

	// matrix = (YAW * PITCH) * ROLL
	matrix[0][0] = cp*cy;
	matrix[0][1] = cp*sy;
	matrix[0][2] = -sp;
	matrix[1][0] = sr*sp*cy+cr*-sy;
	matrix[1][1] = sr*sp*sy+cr*cy;
	matrix[1][2] = sr*cp;
	matrix[2][0] = (cr*sp*cy+-sr*-sy);
	matrix[2][1] = (cr*sp*sy+-sr*cy);
	matrix[2][2] = cr*cp;
	matrix[0][3] = 0.0;
	matrix[1][3] = 0.0;
	matrix[2][3] = 0.0;
}
 
void R_ConcatTransforms (const float in1[3*4], const float in2[3*4], float out[3*4]) // Good
{
	out[0*4+0] = in1[0*4+0] * in2[0*4+0] + in1[0*4+1] * in2[1*4+0] + in1[0*4+2] * in2[2*4+0];
	out[0*4+1] = in1[0*4+0] * in2[0*4+1] + in1[0*4+1] * in2[1*4+1] + in1[0*4+2] * in2[2*4+1];
	out[0*4+2] = in1[0*4+0] * in2[0*4+2] + in1[0*4+1] * in2[1*4+2] + in1[0*4+2] * in2[2*4+2];
	out[0*4+3] = in1[0*4+0] * in2[0*4+3] + in1[0*4+1] * in2[1*4+3] + in1[0*4+2] * in2[2*4+3] + in1[0*4+3];
	out[1*4+0] = in1[1*4+0] * in2[0*4+0] + in1[1*4+1] * in2[1*4+0] + in1[1*4+2] * in2[2*4+0];
	out[1*4+1] = in1[1*4+0] * in2[0*4+1] + in1[1*4+1] * in2[1*4+1] + in1[1*4+2] * in2[2*4+1];
	out[1*4+2] = in1[1*4+0] * in2[0*4+2] + in1[1*4+1] * in2[1*4+2] + in1[1*4+2] * in2[2*4+2];
	out[1*4+3] = in1[1*4+0] * in2[0*4+3] + in1[1*4+1] * in2[1*4+3] + in1[1*4+2] * in2[2*4+3] + in1[1*4+3];
	out[2*4+0] = in1[2*4+0] * in2[0*4+0] + in1[2*4+1] * in2[1*4+0] + in1[2*4+2] * in2[2*4+0];
	out[2*4+1] = in1[2*4+0] * in2[0*4+1] + in1[2*4+1] * in2[1*4+1] + in1[2*4+2] * in2[2*4+1];
	out[2*4+2] = in1[2*4+0] * in2[0*4+2] + in1[2*4+1] * in2[1*4+2] + in1[2*4+2] * in2[2*4+2];
	out[2*4+3] = in1[2*4+0] * in2[0*4+3] + in1[2*4+1] * in2[1*4+3] + in1[2*4+2] * in2[2*4+3] + in1[2*4+3];
}

void VectorRotate (const float in1[3*4],const float in2[3*4],float out[3*4])	// Good
{
	out[0*4+0] = in1[0*4+0] * in2[0*4+0] + in1[0*4+1] * in2[0*4+1] + in1[0*4+2] * in2[0*4+2];
	out[0*4+1] = in1[0*4+0] * in2[0*4+4] + in1[0*4+1] * in2[0*4+5] + in1[0*4+2] * in2[0*4+6];
	out[0*4+2] = in1[0*4+0] * in2[0*4+8] + in1[0*4+1] * in2[0*4+9] + in1[0*4+2] * in2[0*4+10];
}


void VectorIRotate (const float in1[3*4],const float in2[3*4],float out[3*4])	// Good
{
	out[0*4+0] = in1[0*4+0] * in2[0*4+0] + in1[0*4+1] * in2[0*4+4] + in1[0*4+2] * in2[0*4+8];
	out[0*4+1] = in1[0*4+0] * in2[0*4+1] + in1[0*4+1] * in2[0*4+5] + in1[0*4+2] * in2[0*4+9];
	out[0*4+2] = in1[0*4+0] * in2[0*4+2] + in1[0*4+1] * in2[0*4+6] + in1[0*4+2] * in2[0*4+10];
}



void VectorTranslate(const float in1[3*4],const float in2[3*4],float out[3*4])	// Good
{
	out[0*4+0] = in1[0*4+0] + in2[0*4+3];
	out[0*4+1] = in1[0*4+1] + in2[1*4+3];
	out[0*4+2] = in1[0*4+2] + in2[2*4+3];
}



void VectorTransform(const float in1[3*4],const float in2[3*4],float out[3*4])	// Good
{
	out[0*4+0] = in1[0*4+0] * in2[0*4+0] + in1[0*4+1] * in2[0*4+1] + in1[0*4+2] * in2[0*4+2] + in2[0*4+3];
	out[0*4+1] = in1[0*4+0] * in2[1*4+0] + in1[0*4+1] * in2[1*4+1] + in1[0*4+2] * in2[1*4+2] + in2[1*4+3];
	out[0*4+2] = in1[0*4+0] * in2[2*4+0] + in1[0*4+1] * in2[2*4+1] + in1[0*4+2] * in2[2*4+2] + in2[2*4+3];
}






void AngleQuaternion(const vec3_t angles, float * quaternion)	// Good
{
	float angle;
	float sr, sp, sy, cr, cp, cy;

	angle	= angles[2] * 0.5;
	sy	= (float)sin(angle);
	cy	= (float)cos(angle);

	angle			= angles[1] * 0.5;
	sp	= (float)sin(angle);
	cp	= (float)cos(angle);

	angle			= angles[0] * 0.5;
	sr	= (float)sin(angle);
	cr	= (float)cos(angle);

	quaternion[0]	= sr * cp * cy - cr * sp * sy;
	quaternion[1]	= cr * sp * cy + sr * cp * sy;
	quaternion[2]	= cr * cp * sy - sr * sp * cy;
	quaternion[3]	= cr * cp * cy + sr * sp * sy;
}



void QuaternionMatrix(const float * quaternion, float (*matrix)[4])
{
	matrix[0][0]	= 1.0 - ( quaternion[1] * 2 * quaternion[1] ) - ( quaternion[2] * 2 * quaternion[2] );
	matrix[1][0]	=       ( quaternion[0] * 2 * quaternion[1] ) + ( quaternion[3] * 2 * quaternion[2] );
	matrix[2][0]	=       ( quaternion[0] * 2 * quaternion[2] ) - ( quaternion[3] * 2 * quaternion[1] );

	matrix[0][1]	=       ( quaternion[0] * 2 * quaternion[1] ) - ( quaternion[3] * 2 * quaternion[2] );
	matrix[1][1]	= 1.0 - ( quaternion[0] * 2 * quaternion[0] ) - ( quaternion[2] * 2 * quaternion[2] );
	matrix[2][1]	=       ( quaternion[1] * 2 * quaternion[2] ) + ( quaternion[3] * 2 * quaternion[0] );

	matrix[0][2]	=       ( quaternion[0] * 2 * quaternion[2] ) + ( quaternion[3] * 2 * quaternion[1] );
	matrix[1][2]	=       ( quaternion[1] * 2 * quaternion[2] ) - ( quaternion[3] * 2 * quaternion[0] );
	matrix[2][2]	= 1.0 - ( quaternion[0] * 2 * quaternion[0] ) - ( quaternion[1] * 2 * quaternion[1] );
}



//**************-***********************
// START REVIEW HERE

void QuaternionSlerp(const float * p, float * q, float t, float * qt)
{
	int i;
	float omega;
	float cosom;
	float sinom;
	float sclp;
	float sclq;
	float a=0;
	float b=0;

	for ( i=0;i<4;i++)
	{
		a += ( p[i] - q[i] ) * ( p[i] - q[i] );
		b += ( p[i] + q[i] ) * ( p[i] + q[i] );
	}

	if ( a > b )
	{
		for ( i=0;i<4;i++)
		{
			q[i] = -q[i];
		}
	}

	cosom = p[0] * q[0] + p[1] * q[1] + p[2] * q[2] + p[3] * q[3];

	if ( ( (double)1 + cosom) > 1 )
	{
		if ( (1 - cosom ) > -1 )
		{
			omega = acos(cosom);
			sinom = sin(omega);
			sclp = sin((1 - t) * omega) / sinom;
			sclq = sin(t * omega) / sinom;
		}
		else
		{
			sclp = 1 - t;
			sclq = t;
		}

		for ( i=0;i<4;i++)
		{
			qt[i] = sclp * p[i] + sclq * q[i];
		}
	}
	else
	{
		qt[0] = -p[1];
		qt[1] = p[0];
		qt[2] = -p[3];
		qt[3] = p[2];
		sclp = sin(( 1 - t ) * 0.5 * M_PI);
		sclq = sin( t * 0.5 * M_PI );

		for ( i=0;i<3;i++)
		{
			qt[i] = sclp * p[i] + sclq * q[i];
		}
	}


}



void FaceNormalize(float * v1, float * v2, float * v3, float * Normal)
{
	float nx = ( v2[1] - v1[1] ) * ( v3[2] - v1[2] ) - ( v3[1] - v1[1] ) * ( v2[2] - v1[2] );
	float ny = ( v2[1] - v1[1] ) * ( v3[2] - v1[2] ) - ( v3[1] - v1[1] ) * ( v2[2] - v1[2] );
	float nz = ( v2[1] - v1[1] ) * ( v3[2] - v1[2] ) - ( v3[1] - v1[1] ) * ( v2[2] - v1[2] );
	double dot = sqrt( nx * nx + ny * ny + nz * nz );
}




/*
void VectorPolar(vec3_t v, float radius, float theta, float phi)
{
 	v[0]=float(radius * cos(theta) * cos(phi));
	v[1]=float(radius * sin(theta) * cos(phi));
	v[2]=float(radius * sin(phi));
}

void VectorSnap(vec3_t v)
{
  for (int i = 0; i < 3; i++)
  {
    v[i] = (float)floor (v[i] + 0.5);
  }
}


void _Vector5Add (vec5_t va, vec5_t vb, vec5_t out)
{
	out[0] = va[0]+vb[0];
	out[1] = va[1]+vb[1];
	out[2] = va[2]+vb[2];
	out[3] = va[3]+vb[3];
	out[4] = va[4]+vb[4];
}

void _Vector5Scale (vec5_t v, vec_t scale, vec5_t out)
{
	out[0] = v[0] * scale;
	out[1] = v[1] * scale;
	out[2] = v[2] * scale;
	out[3] = v[3] * scale;
	out[4] = v[4] * scale;
}

void _Vector53Copy (vec5_t in, vec3_t out)
{
	out[0] = in[0];
	out[1] = in[1];
	out[2] = in[2];
}
*/





/*void VectorToAngles( vec3_t vec, vec3_t angles )
   {
   float forward;
   float yaw, pitch;
   
   if ( ( vec[ 0 ] == 0 ) && ( vec[ 1 ] == 0 ) )
      {
      yaw = 0;
      if ( vec[ 2 ] > 0 )
         {
         pitch = 90;
         }
      else
         {
         pitch = 270;
         }
      }
   else
      {
      yaw = (float)( atan2( vec[ 1 ], vec[ 0 ] ) * 180 / M_PI );
      if ( yaw < 0 )
         {
         yaw += 360;
         }

      forward = ( float )sqrt( vec[ 0 ] * vec[ 0 ] + vec[ 1 ] * vec[ 1 ] );
      pitch = (float) ( atan2( vec[ 2 ], forward ) * 180 / M_PI );
      if ( pitch < 0 )
         {
         pitch += 360;
         }
      }

   angles[ 0 ] = pitch;
   angles[ 1 ] = yaw;
   angles[ 2 ] = 0;
   }*/

/*
float VectorLength(vec3_t v)	// Good
{
	int i;
	float length;

	length=0;
	for ( i=0;i<3;i++)
	{
		length= length+(v[i]*v[i]);
	}

	return length=sqrtf(length);
}


BOOL VectorCompare(vec3_t v1, vec3_t v2) // Epsilon
{
	for (int i = 0; i <3; i++)
	{
		if (fabs(v1[i] - v2[i]) > EPSILON)	// Change Value of Epsilon
		{
			return	FALSE;
		}
	}
			
	return	TRUE;
}

void CrossProduct(vec3_t Vector1, vec3_t Vector2, vec3_t Cross)
{
	Cross[0]	= Vector1[1] * Vector2[2] - Vector1[2] * Vector2[1];
	Cross[1]	= Vector1[2] * Vector2[0] - Vector1[0] * Vector2[2];
	Cross[2]	= Vector1[0] * Vector2[1] - Vector1[1] * Vector2[0];
}

void R_ConcatTransforms(const float In1[3][4], const float In2[3][4], float Out[3][4])
{
	Out[0][0]	= In1[0][0] * In2[0][0] + In1[0][1] * In2[1][0] + In1[0][2] * In2[2][0];
	Out[0][1]	= In1[0][0] * In2[0][1] + In1[0][1] * In2[1][1] + In1[0][2] * In2[2][1];
	Out[0][2]	= In1[0][0] * In2[0][2] + In1[0][1] * In2[1][2] + In1[0][2] * In2[2][2];
	Out[0][3]	= In1[0][0] * In2[0][3] + In1[0][1] * In2[1][3] + In1[0][2] * In2[2][3] + In1[0][3];
	Out[1][0]	= In1[1][0] * In2[0][0] + In1[1][1] * In2[1][0] + In1[1][2] * In2[2][0];
	Out[1][1]	= In1[1][0] * In2[0][1] + In1[1][1] * In2[1][1] + In1[1][2] * In2[2][1];
	Out[1][2]	= In1[1][0] * In2[0][2] + In1[1][1] * In2[1][2] + In1[1][2] * In2[2][2];
	Out[1][3]	= In1[1][0] * In2[0][3] + In1[1][1] * In2[1][3] + In1[1][2] * In2[2][3] + In1[1][3];
	Out[2][0]	= In1[2][0] * In2[0][0] + In1[2][1] * In2[1][0] + In1[2][2] * In2[2][0];
	Out[2][1]	= In1[2][0] * In2[0][1] + In1[2][1] * In2[1][1] + In1[2][2] * In2[2][1];
	Out[2][2]	= In1[2][0] * In2[0][2] + In1[2][1] * In2[1][2] + In1[2][2] * In2[2][2];
	Out[2][3]	= In1[2][0] * In2[0][3] + In1[2][1] * In2[1][3] + In1[2][2] * In2[2][3] + In1[2][3];
}

// rotate by the inverse of the matrix
void VectorIRotate(const vec3_t In1, const float In2[3][4], vec3_t Out)
{
	Out[0]	= In1[0] * In2[0][0] + In1[1] * In2[1][0] + In1[2] * In2[2][0];
	Out[1]	= In1[0] * In2[0][1] + In1[1] * In2[1][1] + In1[2] * In2[2][1];
	Out[2]	= In1[0] * In2[0][2] + In1[1] * In2[1][2] + In1[2] * In2[2][2];
}
void VectorTransform(const vec3_t In1, const float In2[3][4], vec3_t Out)
{
	Out[0]	= DotProduct(In1, In2[0]) + In2[0][3];
	Out[1]	= DotProduct(In1, In2[1]) + In2[1][3];
	Out[2]	= DotProduct(In1, In2[2]) + In2[2][3];
}

void AngleQuaternion(const vec3_t Angles, vec4_t Quaternion)
{
	float	Angle	= Angles[0] * 0.5f;
	float	Sin0	= (float)sin(Angle);
	float	Cos0	= (float)cos(Angle);

	Angle			= Angles[1] * 0.5f;
	float	Sin1	= (float)sin(Angle);
	float	Cos1	= (float)cos(Angle);

	Angle			= Angles[2] * 0.5f;
	float	Sin2	= (float)sin(Angle);
	float	Cos2	= (float)cos(Angle);

	Quaternion[0]	= Sin0 * Cos1 * Cos2 - Cos0 * Sin1 * Sin2;
	Quaternion[1]	= Cos0 * Sin1 * Cos2 + Sin0 * Cos1 * Sin2;
	Quaternion[2]	= Cos0 * Cos1 * Sin2 - Sin0 * Sin1 * Cos2;
	Quaternion[3]	= Cos0 * Cos1 * Cos2 + Sin0 * Sin1 * Sin2;
}

void QuaternionMatrix(const vec4_t Quaternion, float (*Matrix)[4])
{
	Matrix[0][0]	= 1.0f - 2.0f * Quaternion[1] * Quaternion[1] - 2.0f * Quaternion[2] * Quaternion[2];
	Matrix[1][0]	= 2.0f * Quaternion[0] * Quaternion[1] + 2.0f * Quaternion[3] * Quaternion[2];
	Matrix[2][0]	= 2.0f * Quaternion[0] * Quaternion[2] - 2.0f * Quaternion[3] * Quaternion[1];

	Matrix[0][1]	= 2.0f * Quaternion[0] * Quaternion[1] - 2.0f * Quaternion[3] * Quaternion[2];
	Matrix[1][1]	= 1.0f - 2.0f * Quaternion[0] * Quaternion[0] - 2.0f * Quaternion[2] * Quaternion[2];
	Matrix[2][1]	= 2.0f * Quaternion[1] * Quaternion[2] + 2.0f * Quaternion[3] * Quaternion[0];

	Matrix[0][2]	= 2.0f * Quaternion[0] * Quaternion[2] + 2.0f * Quaternion[3] * Quaternion[1];
	Matrix[1][2]	= 2.0f * Quaternion[1] * Quaternion[2] - 2.0f * Quaternion[3] * Quaternion[0];
	Matrix[2][2]	= 1.0f - 2.0f * Quaternion[0] * Quaternion[0] - 2.0f * Quaternion[1] * Quaternion[1];
}

void QuaternionSlerp(const vec4_t In1, vec4_t In2, float Time, vec4_t Out)
{
	float	Slerp1;
	float	Slerp2;

	// Decide if one of the quaternions is backwards
	float Value1	= 0.0f;
	float Value2	= 0.0f;
	DWORD Loop;
	for (Loop = 0; Loop < 4; Loop++)
	{
		Value1	+= (In1[Loop] - In2[Loop]) * (In1[Loop] - In2[Loop]);
		Value2	+= (In1[Loop] + In2[Loop]) * (In1[Loop] + In2[Loop]);
	}
	
	if (Value1 > Value2)
	{
		for (Loop = 0; Loop < 4; Loop++)
		{
			In2[Loop]	= -In2[Loop];
		}
	}

	float	CosAngle	= In1[0] * In2[0] + In1[1] * In2[1] + In1[2] * In2[2] + In1[3] * In2[3];

	if ((1.0f + CosAngle) > 0.00000001f)
	{
		if ((1.0f - CosAngle) > 0.00000001f)
		{
			float	Omega		= (float)acos(CosAngle);
			float	SinAngle	= (float)sin(Omega);
			
			Slerp1				= (float)sin((1.0f - Time) * Omega) / SinAngle;
			Slerp2				= (float)sin(Time * Omega) / SinAngle;
		}
		
		else
		{
			Slerp1				= 1.0f - Time;
			Slerp2				= Time;
		}
		
		for (Loop = 0; Loop < 4; Loop++)
		{
			Out[Loop]	= Slerp1 * In1[Loop] + Slerp2 * In2[Loop];
		}
	}
	
	else
	{
		Out[0]	= -In1[1];
		Out[1]	= In1[0];
		Out[2]	= -In1[3];
		Out[3]	= In1[2];
		Slerp1	= (float)sin((1.0f - Time) * 0.5f * Q_PI);
		Slerp2	= (float)sin(Time * 0.5f * Q_PI);
		
		for (Loop = 0; Loop < 3; Loop++)
		{
			Out[Loop]	= Slerp1 * In1[Loop] + Slerp2 * Out[Loop];
		}
	}
}
*/
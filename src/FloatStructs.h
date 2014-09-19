#ifndef FROST_FLOAT_STRUCTS_H
#define FROST_FLOAT_STRUCTS_H

namespace Frost
{
	struct FLOAT3
	{
		float _x;
		float _y;
		float _z;
	};

	struct FLOAT4
	{
		float _x;
		float _y;
		float _z;
		float _w;
	};

	struct FLOAT4X4
	{
		union
		{
			float m[4][4];
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			FLOAT4 rows[4];
		};
	};
}

#endif

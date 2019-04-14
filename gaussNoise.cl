#define PI 3.1415926
__kernel void gaussNoiseCL(__global int *In, __global int* Out, int Height, int Width, __global float *r1, __global float *r2, int k)
{
	int x = get_global_id(0);
	int y = get_global_id(1);
	
	if (x < Height&& x > 0 && y < Width  && y > 0)
	{
		float rand1 = r1[x * Width + y];
		float rand2 = r2[x * Width + y];
		//if(rand1 < 1e-100) 
		//	rand1 = 1e-100;
		float x1 = sqrt(-2 * log(rand1));
		float x2 = cos(rand2 * 2 * PI);
		float gn = x1*x2;
		int temp = In[x * Width + y] + k * gn;
        Out[x * Width + y] = (temp > 0 && temp < 255) ? temp : (temp>255 ? 255 : 0);
		temp = 0;
	}
}
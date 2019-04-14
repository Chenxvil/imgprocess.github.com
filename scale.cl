
double BiCubicNum(float x)
{
  //float a = -0.5f;
  float abs_x = x > 0 ? x : -x;
  if( abs_x <= 1)
    return  1.5f  * abs_x*abs_x*abs_x - 2.5f * x*x + 1;
  if( 1.0f < abs_x && abs_x < 2.0f )
    return -0.5f * abs_x*abs_x*abs_x + 2.5f * x*x - 4 * abs_x + 2.0f;
  return 0;
}

__kernel void scaleCL(__global int *In, __global int* Out, int Width, int Height, float Scale)
{
	int window[16];
	int x = get_global_id(0);
	int y = get_global_id(1);
	
	if (x < Scale * Width && x > 0 && y < Scale * Height && y > 0)
	{
	  float srcx = ((float)x) / Scale;
      float srcy = ((float)y) / Scale;
      int x_i = floor(srcx);
      int y_i = floor(srcy);
      float x_dec = srcx - x_i;
      float y_dec = srcy - y_i;
 
	   float k_i[4],k_j[4];
	   k_i[0] = BiCubicNum( 1 + x_dec );
       k_i[1] = BiCubicNum( x_dec );
       k_i[2] = BiCubicNum( 1 - x_dec);
       k_i[3] = BiCubicNum( 2 - x_dec);

       k_j[0] = BiCubicNum( 1 + y_dec );
       k_j[1] = BiCubicNum( y_dec );
       k_j[2] = BiCubicNum( 1 - y_dec);
       k_j[3] = BiCubicNum( 2 - y_dec);

	 	window[0] = (y_i < 1 || x_i < 1 )                ? 0 : In[(y_i - 1)* Width + x_i - 1];
	 	window[1] = (y_i < 1 || x_i < 0 )                ? 0 : In[(y_i - 1)* Width + x_i];
	 	window[2] = (y_i < 1 || x_i > Width - 1)         ? 0 : In[(y_i - 1)* Width + x_i + 1];
	 	window[3] = (y_i < 1 || x_i > Width - 2)         ? 0 : In[(y_i - 1)* Width + x_i + 2];
	 	window[4] = (y_i < 0 || x_i < 1 )                ? 0 : In[(y_i)* Width + x_i - 1];
	 	window[5] = (y_i < 0 || x_i < 0 )                ? 0 : In[(y_i)* Width + x_i];
	 	window[6] = (y_i < 0 || x_i > Width - 1)         ? 0 : In[(y_i)* Width + x_i + 1];
	 	window[7] = (y_i < 0 || x_i > Width - 2)         ? 0 : In[(y_i)* Width + x_i + 2];
	 	window[8] = (y_i > Height - 1|| x_i < 1 )        ? 0 : In[(y_i + 1)* Width + x_i - 1];
	 	window[9] = (y_i > Height - 1|| x_i < 0 )        ? 0 : In[(y_i + 1)* Width + x_i];
	 	window[10]= (y_i > Height - 1|| x_i > Width - 1) ? 0 : In[(y_i + 1)* Width + x_i + 1];
	 	window[11]= (y_i > Height - 1|| x_i > Width - 2) ? 0 : In[(y_i + 1)* Width + x_i + 2];
	 	window[12]= (y_i > Height - 2|| x_i < 1 )        ? 0 : In[(y_i + 2)* Width + x_i - 1];
	 	window[13]= (y_i > Height - 2|| x_i < 0 )        ? 0 : In[(y_i + 2)* Width + x_i];
	 	window[14]= (y_i > Height - 2|| x_i > Width - 1) ? 0 : In[(y_i + 2)* Width + x_i + 1];
	 	window[15]= (y_i > Height - 2|| x_i > Width - 2) ? 0 : In[(y_i + 2)* Width + x_i + 2];
	 	int sum = 0;
	 	for (unsigned int j = 0; j < 16; j++)
	 	{
	 		 sum+=window[j]*k_i[j/4]*k_j[j%4];
		 }
		 Out[y * (int)(Scale * Width) + x] = sum;
		 //Out[y * Width + x] = 0;
		sum = 0;
		}
}
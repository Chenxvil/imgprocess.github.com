
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
__kernel void rotatCL(__global int *In, __global int* Out, int Height, int Width, int nHeight, int nWidth, float degree)
{
    int window[16];
	int x = get_global_id(0);
	int y = get_global_id(1);
	
	if (x < nHeight&& x > 0 && y < nWidth  && y > 0)
	{
	//std::cout<<y<<std::endl;
	  float srcx = (x - nHeight * 0.5) * cos(degree) - (y - nWidth * 0.5) * sin(degree) + Height * 0.5;
      float srcy = (x - nHeight * 0.5) * sin(degree) + (y - nWidth * 0.5) * cos(degree) + Width * 0.5;
	  if(srcx < 0 || srcy < 0 || srcx >= Height  || srcy >= Width)
      {
         Out[ x*nWidth + y] = 0;
		  return;
      }
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

	 	window[0] = (x_i < 1 || y_i < 1 )                ? 0 : In[(x_i - 1)* Width + y_i - 1];
        window[1] = (x_i < 1 || y_i < 0 )                ? 0 : In[(x_i - 1)* Width + y_i];
        window[2] = (x_i < 1 || y_i > Width - 2)         ? 0 : In[(x_i - 1)* Width + y_i + 1];
        window[3] = (x_i < 1 || y_i > Width - 3)         ? 0 : In[(x_i - 1)* Width + y_i + 2];
        window[4] = (x_i < 0 || y_i < 1 )                ? 0 : In[(x_i)* Width + y_i - 1];
        window[5] = (x_i < 0 || y_i < 0 )                ? 0 : In[(x_i)* Width + y_i];
        window[6] = (x_i < 0 || y_i > Width - 2)         ? 0 : In[(x_i)* Width + y_i + 1];
        window[7] = (x_i < 0 || y_i > Width - 3)         ? 0 : In[(x_i)* Width + y_i + 2];
        window[8] = (x_i > Height - 2|| y_i < 1 )        ? 0 : In[(x_i + 1)* Width + y_i - 1];
        window[9] = (x_i > Height - 2|| y_i < 0 )        ? 0 : In[(x_i + 1)* Width + y_i];
        window[10]= (x_i > Height - 2|| y_i > Width - 2) ? 0 : In[(x_i + 1)* Width + y_i + 1];
        window[11]= (x_i > Height - 2|| y_i > Width - 3) ? 0 : In[(x_i + 1)* Width + y_i + 2];
        window[12]= (x_i > Height - 3|| y_i < 1 )        ? 0 : In[(x_i + 2)* Width + y_i - 1];
        window[13]= (x_i > Height - 3|| y_i < 0 )        ? 0 : In[(x_i + 2)* Width + y_i];
        window[14]= (x_i > Height - 3|| y_i > Width - 2) ? 0 : In[(x_i + 2)* Width + y_i + 1];
        window[15]= (x_i > Height - 3|| y_i > Width - 3) ? 0 : In[(x_i + 2)* Width + y_i + 2];
	 	int sum = 0;
	 	for (unsigned int j = 0; j < 16; j++)
	 	{
	 		 sum+=window[j]*k_i[j/4]*k_j[j%4];
		 }
		 Out[x * nWidth + y] = sum;
		sum = 0;
		}
}
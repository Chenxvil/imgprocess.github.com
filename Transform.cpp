#include"Transform.h"

size_t Transform::RoundUP(int groupSize, int globalSize)
{
	int r = globalSize % groupSize;
	if (r == 0)
	{
		return globalSize;
	}
	else
	{
		return globalSize + groupSize - r;
	}
}
char* Transform::LoadProgSource(const char* cFilename, const char* cPreamble, size_t* szFinalLength)
{
	FILE* pFileStream = NULL;
	size_t szSourceLength;

	// open the OpenCL source code file  
	pFileStream = fopen(cFilename, "rb");
	if (pFileStream == NULL)
	{
		return NULL;
	}

	size_t szPreambleLength = strlen(cPreamble);

	// get the length of the source code  
	fseek(pFileStream, 0, SEEK_END);
	szSourceLength = ftell(pFileStream);
	fseek(pFileStream, 0, SEEK_SET);

	// allocate a buffer for the source code string and read it in  
	char* cSourceString = (char *)malloc(szSourceLength + szPreambleLength + 1);
	memcpy(cSourceString, cPreamble, szPreambleLength);
	if (fread((cSourceString)+szPreambleLength, szSourceLength, 1, pFileStream) != 1)
	{
		fclose(pFileStream);
		free(cSourceString);
		return 0;
	}

	// close the file and return the total length of the combined (preamble + source) string  
	fclose(pFileStream);
	if (szFinalLength != 0)
	{
		*szFinalLength = szSourceLength + szPreambleLength;
	}
	cSourceString[szSourceLength + szPreambleLength] = '\0';

	return cSourceString;
}

Mat Transform::scaleProcess(Mat* src, float scale)
{
	int height = src->rows;
	int width = src->cols;
	int length = height * width;
	int *pixelInR = (int*)malloc(length * sizeof(int));
	int *pixelInG = (int*)malloc(length * sizeof(int));
	int *pixelInB = (int*)malloc(length * sizeof(int));
	int *pixelOutR = (int*)malloc(scale * scale * length * sizeof(int));
	int *pixelOutG = (int*)malloc(scale * scale * length * sizeof(int));
	int *pixelOutB = (int*)malloc(scale * scale * length * sizeof(int));
	int index = 0;

	for (int x = 0; x < height; x++)
	{
		for (int y = 0; y < width; y++)
		{
			pixelInR[index] = src->at<Vec3b>(x, y)[0];
			pixelInG[index] = src->at<Vec3b>(x, y)[1];
			pixelInB[index] = src->at<Vec3b>(x, y)[2];
			index++;
		}
	}

	scale_cl(pixelInR, pixelOutR, width, height, scale);
	scale_cl(pixelInG, pixelOutG, width, height, scale);
	scale_cl(pixelInB, pixelOutB, width, height, scale);

	Mat dst(scale*height, scale*width, src->type());
	index = 0;
	for (int x = 0; x < dst.rows; x++)
	{
		for (int y = 0; y < dst.cols; y++)
		{
			dst.at<Vec3b>(x, y)[0] = pixelOutR[index];
			dst.at<Vec3b>(x, y)[1] = pixelOutG[index];
			dst.at<Vec3b>(x, y)[2] = pixelOutB[index];
			index++;
		}
	}
	free(pixelInR);
	free(pixelInG);
	free(pixelInB);
	free(pixelOutR);
	free(pixelOutG);
	free(pixelOutB);
	return dst;
}
void Transform::scale_cl(int* in, int* out, int width, int height, float scale)
{
	//step 1:get platform;
	cl_int ret;														//errcode;
	cl_uint num_platforms;											//用于保存获取到的platforms数量;
	ret = clGetPlatformIDs(0, NULL, &num_platforms);
	if ((CL_SUCCESS != ret) || (num_platforms < 1))
	{
		QString errMsg;
		errMsg.append("Error getting platform number: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}
	cl_platform_id platform_id = NULL;
	ret = clGetPlatformIDs(1, &platform_id, NULL);					//获取第一个platform的id;
	if (CL_SUCCESS != ret)
	{
		QString errMsg;
		errMsg.append("Error getting platform number: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	//step 2:get device ;
	cl_uint num_devices;
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
	if ((CL_SUCCESS != ret) || (num_devices < 1))
	{
		QString errMsg;
		errMsg.append("Error getting GPU device number: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}
	cl_device_id device_id = NULL;
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	if (CL_SUCCESS != ret)
	{
		QString errMsg;
		errMsg.append("Error getting GPU device id: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	//step 3:create context;
	cl_context_properties props[] =
	{
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform_id, 0
	};
	cl_context context = NULL;
	context = clCreateContext(props, 1, &device_id, NULL, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == context))
	{
		QString errMsg;
		errMsg.append("Error createing context: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	//step 4:create command queue;						//一个device有多个queue，queue之间并行执行
	cl_command_queue command_queue = NULL;
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	if ((CL_SUCCESS != ret) || (NULL == command_queue))
	{
		QString errMsg;
		errMsg.append("Error createing command queue: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	//step 5:create memory object;						//缓存类型（buffer），图像类型（iamge）

	cl_mem mem_obj = NULL;
	cl_mem mem_objout = NULL;

	//create opencl memory object using host ptr
	mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, width * height * sizeof(int), in, &ret);
	mem_objout = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, scale * scale * width * height * sizeof(int), out, &ret);
	if ((CL_SUCCESS != ret) || (NULL == mem_obj))
	{

		QString errMsg;
		errMsg.append("Error creating memory object: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	//step 6:create program;
	size_t szKernelLength = 0;
	const char* oclSourceFile = "scale.cl";
	const char* kernelSource = LoadProgSource(oclSourceFile, "", &szKernelLength);
	if (kernelSource == NULL)
	{
		QString errMsg;
		errMsg.append("Error loading source file: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	//create program
	cl_program program = NULL;
	program = clCreateProgramWithSource(context, 1, (const char **)&kernelSource, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == program))
	{
		QString errMsg;
		errMsg.append("Error creating program: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	//build program 
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	if (CL_SUCCESS != ret)
	{
		size_t len;
		char buffer[8 * 1024];
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), &buffer, &len);
		//QString errMsg = (QString)buffer;
		for (int i = 0; i < 300; i++)
			std::cout << buffer[i];
		return;
	}

	//step 7:create kernel;
	cl_kernel kernel = NULL;
	kernel = clCreateKernel(program, "scaleCL", &ret);
	if ((CL_SUCCESS != ret) || (NULL == kernel))
	{
		QString errMsg;
		errMsg.append("Error creating kernel: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	//step 8:set kernel arguement;
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)& mem_obj);
	if (CL_SUCCESS != ret)
	{
		QString errMsg;
		errMsg.append("Error setting kernel arguement 0: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)& mem_objout);
	if (CL_SUCCESS != ret)
	{
		QString errMsg;
		errMsg.append("Error setting kernel arguement 1: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}
	ret = clSetKernelArg(kernel, 2, sizeof(int), (void*)& width);
	if (CL_SUCCESS != ret)
	{
		QString errMsg;
		errMsg.append("Error setting kernel arguement 2: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}
	ret = clSetKernelArg(kernel, 3, sizeof(int), (void*)& height);
	if (CL_SUCCESS != ret)
	{
		QString errMsg;
		errMsg.append("Error setting kernel arguement 3: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}
	ret = clSetKernelArg(kernel, 4, sizeof(float), (void*)& scale);
	if (CL_SUCCESS != ret)
	{
		QString errMsg;
		errMsg.append("Error setting kernel arguement 4 ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}
	//step 9:set work group size;  							//<---->dimBlock\dimGrid
	cl_uint work_dim = 2;
	size_t local_work_size[2] = { 4, 4 };
	size_t global_work_size[2] = { RoundUP(local_work_size[0], width * scale),RoundUP(local_work_size[1], height * scale) };

	//step 10:run kernel;				//put kernel and work-item arugement into queue and excute;
	ret = clEnqueueNDRangeKernel(command_queue, kernel, work_dim, NULL, global_work_size, local_work_size, 0, NULL, NULL);
	if (CL_SUCCESS != ret)
	{
		QString errMsg;
		errMsg.append("Error enqueue NDRange: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	//step 11:get result;
	int *device_buffer = (int *)clEnqueueMapBuffer(command_queue, mem_objout, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, (scale * scale * width * height * sizeof(int)), 0, NULL, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == device_buffer))
	{
		QString errMsg;
		errMsg.append("Error map buffer: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	memcpy(out, device_buffer, scale * scale * width * height * sizeof(int));

	//step 12:release all resource;
	if (NULL != mem_objout)
		clReleaseMemObject(mem_objout);
	if (NULL != kernel)
		clReleaseKernel(kernel);
	if (NULL != program)
		clReleaseProgram(program);
	if (NULL != mem_obj)
		clReleaseMemObject(mem_obj);
	if (NULL != command_queue)
		clReleaseCommandQueue(command_queue);
	if (NULL != context)
		clReleaseContext(context);
}

Mat Transform::rotatProcess(Mat* src, float degree)
{
	degree /= (180 / PI);
	int height = src->rows;
	int width = src->cols;
	int length = height * width;
	int nheight = width * abs(sin(degree)) + height * abs(cos(degree));
	int nwidth = width * abs(cos(degree)) + height * abs(sin(degree));
	int nlength = nheight * nwidth;
	int *pixelInR = (int*)malloc(length * sizeof(int));
	int *pixelInG = (int*)malloc(length * sizeof(int));
	int *pixelInB = (int*)malloc(length * sizeof(int));
	int *pixelOutR = (int*)malloc(nlength * sizeof(int));
	int *pixelOutG = (int*)malloc(nlength * sizeof(int));
	int *pixelOutB = (int*)malloc(nlength * sizeof(int));
	int index = 0;
	for (int x = 0; x < height; x++)
	{
		for (int y = 0; y < width; y++)
		{
			pixelInR[index] = src->at<Vec3b>(x, y)[0];
			pixelInG[index] = src->at<Vec3b>(x, y)[1];
			pixelInB[index] = src->at<Vec3b>(x, y)[2];
			index++;
		}
	}
	rotat_cl(pixelInR, pixelOutR, height, width, nheight, nwidth, degree);
	rotat_cl(pixelInG, pixelOutG, height, width, nheight, nwidth, degree);
	rotat_cl(pixelInB, pixelOutB, height, width, nheight, nwidth, degree);
	Mat dst(nheight, nwidth, src->type());
	index = 0;
	for (int x = 0; x < nheight; x++)
	{
		for (int y = 0; y < nwidth; y++)
		{
			dst.at<Vec3b>(x, y)[0] = pixelOutR[index];
			dst.at<Vec3b>(x, y)[1] = pixelOutG[index];
			dst.at<Vec3b>(x, y)[2] = pixelOutB[index];
			index++;
		}
	}
	free(pixelInR);
	free(pixelInG);
	free(pixelInB);
	free(pixelOutR);
	free(pixelOutG);
	free(pixelOutB);
	return dst;
}
void Transform::rotat_cl(int* in, int* out, int height, int width, int nheight, int nwidth, float scale)
{
	//step 1:get platform;
	cl_int ret;														//errcode;
	cl_uint num_platforms;											//用于保存获取到的platforms数量;
	ret = clGetPlatformIDs(0, NULL, &num_platforms);
	if ((CL_SUCCESS != ret) || (num_platforms < 1))
	{
		QString errMsg;
		errMsg.append("Error getting platform number: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}
	cl_platform_id platform_id = NULL;
	ret = clGetPlatformIDs(1, &platform_id, NULL);					//获取第一个platform的id;
	if (CL_SUCCESS != ret)
	{
		QString errMsg;
		errMsg.append("Error getting platform number: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	//step 2:get device ;
	cl_uint num_devices;
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
	if ((CL_SUCCESS != ret) || (num_devices < 1))
	{
		QString errMsg;
		errMsg.append("Error getting GPU device number: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}
	cl_device_id device_id = NULL;
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	if (CL_SUCCESS != ret)
	{
		QString errMsg;
		errMsg.append("Error getting GPU device id: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	//step 3:create context;
	cl_context_properties props[] =
	{
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform_id, 0
	};
	cl_context context = NULL;
	context = clCreateContext(props, 1, &device_id, NULL, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == context))
	{
		QString errMsg;
		errMsg.append("Error createing context: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	//step 4:create command queue;						//一个device有多个queue，queue之间并行执行
	cl_command_queue command_queue = NULL;
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	if ((CL_SUCCESS != ret) || (NULL == command_queue))
	{
		QString errMsg;
		errMsg.append("Error createing command queue: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	//step 5:create memory object;						//缓存类型（buffer），图像类型（iamge）

	cl_mem mem_obj = NULL;
	cl_mem mem_objout = NULL;

	//create opencl memory object using host ptr
	mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, width * height * sizeof(int), in, &ret);
	mem_objout = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, nwidth * nheight * sizeof(int), out, &ret);
	if ((CL_SUCCESS != ret) || (NULL == mem_obj))
	{

		QString errMsg;
		errMsg.append("Error creating memory object: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	//step 6:create program;
	size_t szKernelLength = 0;
	const char* oclSourceFile = "rotat.cl";
	const char* kernelSource = LoadProgSource(oclSourceFile, "", &szKernelLength);
	if (kernelSource == NULL)
	{
		QString errMsg;
		errMsg.append("Error loading source file: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	//create program
	cl_program program = NULL;
	program = clCreateProgramWithSource(context, 1, (const char **)&kernelSource, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == program))
	{
		QString errMsg;
		errMsg.append("Error creating program: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	//build program 
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	if (CL_SUCCESS != ret)
	{
		size_t len;
		char buffer[8 * 1024];
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), &buffer, &len);
		for (int i = 0; i < 300; i++)
			std::cout << buffer[i];
		return;
	}

	//step 7:create kernel;
	cl_kernel kernel = NULL;
	kernel = clCreateKernel(program, "rotatCL", &ret);
	if ((CL_SUCCESS != ret) || (NULL == kernel))
	{
		QString errMsg;
		errMsg.append("Error creating kernel: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	//step 8:set kernel arguement;
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)& mem_obj);
	if (CL_SUCCESS != ret)
	{
		QString errMsg;
		errMsg.append("Error setting kernel arguement 0: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)& mem_objout);
	if (CL_SUCCESS != ret)
	{
		QString errMsg;
		errMsg.append("Error setting kernel arguement 1: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}
	ret = clSetKernelArg(kernel, 2, sizeof(int), (void*)& height);
	if (CL_SUCCESS != ret)
	{
		QString errMsg;
		errMsg.append("Error setting kernel arguement 2: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}
	ret = clSetKernelArg(kernel, 3, sizeof(int), (void*)& width);
	if (CL_SUCCESS != ret)
	{
		QString errMsg;
		errMsg.append("Error setting kernel arguement 3: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}
	ret = clSetKernelArg(kernel, 4, sizeof(int), (void*)& nheight);
	if (CL_SUCCESS != ret)
	{
		QString errMsg;
		errMsg.append("Error setting kernel arguement 4: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}
	ret = clSetKernelArg(kernel, 5, sizeof(int), (void*)& nwidth);
	if (CL_SUCCESS != ret)
	{
		QString errMsg;
		errMsg.append("Error setting kernel arguement 5: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}
	ret = clSetKernelArg(kernel, 6, sizeof(float), (void*)& scale);
	if (CL_SUCCESS != ret)
	{
		QString errMsg;
		errMsg.append("Error setting kernel arguement 6 ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}
	//step 9:set work group size;  							//<---->dimBlock\dimGrid
	cl_uint work_dim = 2;
	size_t local_work_size[2] = { 16, 16 };
	size_t global_work_size[2] = { RoundUP(local_work_size[0], nheight),RoundUP(local_work_size[1], nwidth) };

	//step 10:run kernel;				//put kernel and work-item arugement into queue and excute;
	ret = clEnqueueNDRangeKernel(command_queue, kernel, work_dim, NULL, global_work_size, local_work_size, 0, NULL, NULL);
	if (CL_SUCCESS != ret)
	{
		QString errMsg;
		errMsg.append("Error enqueue NDRange: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	//step 11:get result;
	int *device_buffer = (int *)clEnqueueMapBuffer(command_queue, mem_objout, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, nwidth * nheight * sizeof(int), 0, NULL, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == device_buffer))
	{
		QString errMsg;
		errMsg.append("Error map buffer: ");
		errMsg.append(ret);
		std::cout << errMsg.toStdString();
		return;
	}

	memcpy(out, device_buffer, nwidth * nheight * sizeof(int));

	//step 12:release all resource;
	if (NULL != mem_objout)
		clReleaseMemObject(mem_objout);
	if (NULL != kernel)
		clReleaseKernel(kernel);
	if (NULL != program)
		clReleaseProgram(program);
	if (NULL != mem_obj)
		clReleaseMemObject(mem_obj);
	if (NULL != command_queue)
		clReleaseCommandQueue(command_queue);
	if (NULL != context)
		clReleaseContext(context);
}

Mat Transform::gaussNoiseProcess(Mat* src, int factor)
{
	int height = src->rows;
	int width = src->cols;
	int length = height * width;
	int* pixelInR = (int*)malloc(length * sizeof(int));
	int* pixelInG = (int*)malloc(length * sizeof(int));
	int* pixelInB = (int*)malloc(length * sizeof(int));
	int* pixelOutR = (int*)malloc(length * sizeof(int));
	int* pixelOutG = (int*)malloc(length * sizeof(int));
	int* pixelOutB = (int*)malloc(length * sizeof(int));
	float* randR1 = (float*)malloc(length * sizeof(float));
	float* randR2 = (float*)malloc(length * sizeof(float));
	float* randG1 = (float*)malloc(length * sizeof(float));
	float* randG2 = (float*)malloc(length * sizeof(float));
	float* randB1 = (float*)malloc(length * sizeof(float));
	float* randB2 = (float*)malloc(length * sizeof(float));
	int index = 0;

	for (int x = 0; x < height; x++)
	{
		for (int y = 0; y < width; y++)
		{
			pixelInR[index] = src->at<Vec3b>(x, y)[0];
			pixelInG[index] = src->at<Vec3b>(x, y)[1];
			pixelInB[index] = src->at<Vec3b>(x, y)[2];
			randR1[index] = rand() / (float)RAND_MAX;
			randR2[index] = rand() / (float)RAND_MAX;
			randG1[index] = rand() / (float)RAND_MAX;
			randG2[index] = rand() / (float)RAND_MAX;
			randB1[index] = rand() / (float)RAND_MAX;
			randB2[index] = rand() / (float)RAND_MAX;
			index++;
		}
	}
	gaussnoise_cl(pixelInR, pixelOutR, height, width, factor, randR1, randR2);
	gaussnoise_cl(pixelInG, pixelOutG, height, width, factor, randG1, randG2);
	gaussnoise_cl(pixelInB, pixelOutB, height, width, factor, randB1, randB2);
	Mat dst(height, width, src->type());
	index = 0;
	for (int x = 0; x < height; x++)
	{
		for (int y = 0; y < width; y++)
		{
			dst.at<Vec3b>(x, y)[0] = pixelOutR[index];
			dst.at<Vec3b>(x, y)[1] = pixelOutG[index];
			dst.at<Vec3b>(x, y)[2] = pixelOutB[index];
			index++;
		}
	}
	free(pixelInR);
	free(pixelInG);
	free(pixelInB);
	free(pixelOutR);
	free(pixelOutG);
	free(pixelOutB);
	free(randR1);
	free(randG1);
	free(randB1);
	free(randR2);
	free(randG2);
	free(randB2);
	return dst;
}
void Transform::gaussnoise_cl(int* in, int* out, int height, int width, int factor, float* r1, float* r2)
{
	{
		//step 1:get platform;
		cl_int ret;														//errcode;
		cl_uint num_platforms;											//用于保存获取到的platforms数量;
		ret = clGetPlatformIDs(0, NULL, &num_platforms);
		if ((CL_SUCCESS != ret) || (num_platforms < 1))
		{
			QString errMsg;
			errMsg.append("Error getting platform number: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}
		cl_platform_id platform_id = NULL;
		ret = clGetPlatformIDs(1, &platform_id, NULL);					//获取第一个platform的id;
		if (CL_SUCCESS != ret)
		{
			QString errMsg;
			errMsg.append("Error getting platform number: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}

		//step 2:get device ;
		cl_uint num_devices;
		ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
		if ((CL_SUCCESS != ret) || (num_devices < 1))
		{
			QString errMsg;
			errMsg.append("Error getting GPU device number: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}
		cl_device_id device_id = NULL;
		ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
		if (CL_SUCCESS != ret)
		{
			QString errMsg;
			errMsg.append("Error getting GPU device id: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}

		//step 3:create context;
		cl_context_properties props[] =
		{
			CL_CONTEXT_PLATFORM, (cl_context_properties)platform_id, 0
		};
		cl_context context = NULL;
		context = clCreateContext(props, 1, &device_id, NULL, NULL, &ret);
		if ((CL_SUCCESS != ret) || (NULL == context))
		{
			QString errMsg;
			errMsg.append("Error createing context: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}

		//step 4:create command queue;						//一个device有多个queue，queue之间并行执行
		cl_command_queue command_queue = NULL;
		command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
		if ((CL_SUCCESS != ret) || (NULL == command_queue))
		{
			QString errMsg;
			errMsg.append("Error createing command queue: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}

		//step 5:create memory object;						//缓存类型（buffer），图像类型（iamge）

		cl_mem mem_obj = NULL;
		cl_mem mem_r1 = NULL;
		cl_mem mem_r2 = NULL;
		cl_mem mem_objout = NULL;

		//create opencl memory object using host ptr
		mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, width * height * sizeof(int), in, &ret);
		mem_r1 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, width * height * sizeof(float), r1, &ret);
		mem_r2 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, width * height * sizeof(float), r2, &ret);
		mem_objout = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, width * height * sizeof(int), out, &ret);

		if ((CL_SUCCESS != ret) || (NULL == mem_obj))
		{

			QString errMsg;
			errMsg.append("Error creating memory object: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}
		if ((CL_SUCCESS != ret) || (NULL == mem_r1))
		{

			QString errMsg;
			errMsg.append("Error creating memory object: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}
		if ((CL_SUCCESS != ret) || (NULL == mem_r2))
		{

			QString errMsg;
			errMsg.append("Error creating memory object: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}

		//step 6:create program;
		size_t szKernelLength = 0;
		const char* oclSourceFile = "gaussNoise.cl";
		const char* kernelSource = LoadProgSource(oclSourceFile, "", &szKernelLength);
		if (kernelSource == NULL)
		{
			QString errMsg;
			errMsg.append("Error loading source file: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}

		//create program
		cl_program program = NULL;
		program = clCreateProgramWithSource(context, 1, (const char **)&kernelSource, NULL, &ret);
		if ((CL_SUCCESS != ret) || (NULL == program))
		{
			QString errMsg;
			errMsg.append("Error creating program: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}

		//build program 
		ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
		if (CL_SUCCESS != ret)
		{
			size_t len;
			char buffer[8 * 1024];
			clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), &buffer, &len);
			//QString errMsg = (QString)buffer;
			for (int i = 0; i < 300; i++)
				std::cout << buffer[i];
			return;
		}

		//step 7:create kernel;
		cl_kernel kernel = NULL;
		kernel = clCreateKernel(program, "gaussNoiseCL", &ret);
		if ((CL_SUCCESS != ret) || (NULL == kernel))
		{
			QString errMsg;
			errMsg.append("Error creating kernel: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}

		//step 8:set kernel arguement;
		ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)& mem_obj);
		if (CL_SUCCESS != ret)
		{
			QString errMsg;
			errMsg.append("Error setting kernel arguement 0: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}
		ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)& mem_objout);
		if (CL_SUCCESS != ret)
		{
			QString errMsg;
			errMsg.append("Error setting kernel arguement 1: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}
		ret = clSetKernelArg(kernel, 2, sizeof(int), (void*)& height);
		if (CL_SUCCESS != ret)
		{
			QString errMsg;
			errMsg.append("Error setting kernel arguement 2: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}
		ret = clSetKernelArg(kernel, 3, sizeof(int), (void*)& width);
		if (CL_SUCCESS != ret)
		{
			QString errMsg;
			errMsg.append("Error setting kernel arguement 3: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}
		ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void*)& mem_r1);
		if (CL_SUCCESS != ret)
		{
			QString errMsg;
			errMsg.append("Error setting kernel arguement 4: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}
		ret = clSetKernelArg(kernel, 5, sizeof(cl_mem), (void*)& mem_r2);
		if (CL_SUCCESS != ret)
		{
			QString errMsg;
			errMsg.append("Error setting kernel arguement 5: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}
		ret = clSetKernelArg(kernel, 6, sizeof(int), (void*)& factor);
		if (CL_SUCCESS != ret)
		{
			QString errMsg;
			errMsg.append("Error setting kernel arguement 6: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}

		//step 9:set work group size;  							//<---->dimBlock\dimGrid
		cl_uint work_dim = 2;
		size_t local_work_size[2] = { 16, 16 };
		size_t global_work_size[2] = { RoundUP(local_work_size[0], height),RoundUP(local_work_size[1], width) };

		//step 10:run kernel;				//put kernel and work-item arugement into queue and excute;
		ret = clEnqueueNDRangeKernel(command_queue, kernel, work_dim, NULL, global_work_size, local_work_size, 0, NULL, NULL);
		if (CL_SUCCESS != ret)
		{
			QString errMsg;
			errMsg.append("Error enqueue NDRange: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}

		//step 11:get result;
		int *device_buffer = (int *)clEnqueueMapBuffer(command_queue, mem_objout, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, width * height * sizeof(int), 0, NULL, NULL, &ret);
		if ((CL_SUCCESS != ret) || (NULL == device_buffer))
		{
			QString errMsg;
			errMsg.append("Error map buffer: ");
			errMsg.append(ret);
			std::cout << errMsg.toStdString();
			return;
		}

		memcpy(out, device_buffer, width * height * sizeof(int));

		//step 12:release all resource;
		if (NULL != kernel)
			clReleaseKernel(kernel);
		if (NULL != program)
			clReleaseProgram(program);
		if (NULL != mem_obj)
			clReleaseMemObject(mem_obj);
		if (NULL != mem_objout)
			clReleaseMemObject(mem_objout);
		if (NULL != mem_r1)
			clReleaseMemObject(mem_r1);
		if (NULL != mem_r2)
			clReleaseMemObject(mem_r2);

		if (NULL != command_queue)
			clReleaseCommandQueue(command_queue);
		if (NULL != context)
			clReleaseContext(context);
	}
}
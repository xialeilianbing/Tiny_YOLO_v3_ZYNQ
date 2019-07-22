/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */
#include <stdio.h>
#include <math.h>

#include "xil_printf.h"
#include "xparameters.h"
#include <xtime_l.h>

//include IP head files
#include "xyolo_conv_top.h"
#include "xyolo_max_pool_top.h"
#include "xyolo_yolo_top.h"
#include "xyolo_acc_top.h"
#include "xyolo_upsamp_top.h"
#include "xaxidma.h"
#include "xaxis_switch.h"


//declare IP instances
XYolo_conv_top yolo_conv_top;
XYolo_conv_top_Config *yolo_conv_top_cfg;
XYolo_max_pool_top yolo_max_pool_top;
XYolo_max_pool_top_Config *yolo_max_pool_top_cfg;
XYolo_yolo_top yolo_yolo_top;
XYolo_yolo_top_Config *yolo_yolo_top_cfg;
XYolo_acc_top yolo_acc_top;
XYolo_acc_top_Config *yolo_acc_top_cfg;
XYolo_upsamp_top yolo_upsamp_top;
XYolo_upsamp_top_Config *yolo_upsamp_top_cfg;
XAxiDma axiDMA_0,axiDMA_1;
XAxiDma_Config *axiDMA_cfg_0,*axiDMA_cfg_1;
XAxis_Switch axis_switch_0,axis_switch_1,axis_switch_2;
XAxis_Switch_Config *axis_switch_cfg_0,*axis_switch_cfg_1,*axis_switch_cfg_2;


#include "group_12_input.h"
#include "group_12_output.h"
#include "group_12_weight_it.h"

void init_yolo_conv()
{
	printf("Initializing yolo_conv_top\n");
	yolo_conv_top_cfg = XYolo_conv_top_LookupConfig(XPAR_YOLO_CONV_TOP_0_DEVICE_ID);
	if(yolo_conv_top_cfg)
	{
		int status = XYolo_conv_top_CfgInitialize(&yolo_conv_top,yolo_conv_top_cfg);
		if(status != XST_SUCCESS)
		{
			printf("Error initializing yolo_conv_top core\n");
		}
	}
}

void init_yolo_max_pool()
{
	printf("Initializing yolo_max_pool_top\n");
	yolo_max_pool_top_cfg = XYolo_max_pool_top_LookupConfig(XPAR_YOLO_MAX_POOL_TOP_0_DEVICE_ID);
	if(yolo_max_pool_top_cfg)
	{
		int status = XYolo_max_pool_top_CfgInitialize(&yolo_max_pool_top,yolo_max_pool_top_cfg);
		if(status != XST_SUCCESS)
		{
			printf("Error initializing yolo_max_pool_top core\n");
		}
	}
}

void init_yolo_yolo()
{
	printf("Initializing yolo_yolo_top\n");
	yolo_yolo_top_cfg = XYolo_yolo_top_LookupConfig(XPAR_YOLO_YOLO_TOP_0_DEVICE_ID);
	if(yolo_yolo_top_cfg)
	{
		int status = XYolo_yolo_top_CfgInitialize(&yolo_yolo_top,yolo_yolo_top_cfg);
		if(status != XST_SUCCESS)
		{
			printf("Error initializing yolo_yolo_top core\n");
		}
	}
}

void init_yolo_acc()
{
	printf("Initializing yolo_acc_top\n");
	yolo_acc_top_cfg = XYolo_acc_top_LookupConfig(XPAR_YOLO_ACC_TOP_0_DEVICE_ID);
	if(yolo_acc_top_cfg)
	{
		int status = XYolo_acc_top_CfgInitialize(&yolo_acc_top,yolo_acc_top_cfg);
		if(status != XST_SUCCESS)
		{
			printf("Error initializing yolo_acc_top core\n");
		}
	}
}

void init_yolo_upsamp()
{
	printf("Initializing yolo_upsamp_top\n");
	yolo_upsamp_top_cfg = XYolo_upsamp_top_LookupConfig(XPAR_YOLO_UPSAMP_TOP_0_DEVICE_ID);
	if(yolo_upsamp_top_cfg)
	{
		int status = XYolo_upsamp_top_CfgInitialize(&yolo_upsamp_top,yolo_upsamp_top_cfg);
		if(status != XST_SUCCESS)
		{
			printf("Error initializing yolo_upsamp_top core\n");
		}
	}
}

void init_axi_dma()
{
	printf("Initializing AxiDMA\n");
	axiDMA_cfg_0 = XAxiDma_LookupConfig(XPAR_AXIDMA_0_DEVICE_ID);
	if(axiDMA_cfg_0)
	{
		int status = XAxiDma_CfgInitialize(&axiDMA_0,axiDMA_cfg_0);
		if(status != XST_SUCCESS)
		{
			printf("Error initializing AxiDMA core 0\n");
		}
	}
	axiDMA_cfg_1 = XAxiDma_LookupConfig(XPAR_AXIDMA_1_DEVICE_ID);
	if(axiDMA_cfg_1)
	{
		int status = XAxiDma_CfgInitialize(&axiDMA_1,axiDMA_cfg_1);
		if(status != XST_SUCCESS)
		{
			printf("Error initializing AxiDMA core 1\n");
		}
	}

	XAxiDma_IntrDisable(&axiDMA_0,XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&axiDMA_0,XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_IntrDisable(&axiDMA_1,XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&axiDMA_1,XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DMA_TO_DEVICE);
}

void init_axis_switch()
{
	printf("Initializing axis switches\n");
	axis_switch_cfg_0 = XAxisScr_LookupConfig(XPAR_AXIS_SWITCH_0_DEVICE_ID);
	if(axis_switch_cfg_0)
	{
		int status = XAxisScr_CfgInitialize(&axis_switch_0,axis_switch_cfg_0,axis_switch_cfg_0->BaseAddress);
		if(status != XST_SUCCESS)
		{
			printf("Error initializing axis switch 0\n");
		}
	}
	axis_switch_cfg_1 = XAxisScr_LookupConfig(XPAR_AXIS_SWITCH_1_DEVICE_ID);
	if(axis_switch_cfg_1)
	{
		int status = XAxisScr_CfgInitialize(&axis_switch_1,axis_switch_cfg_1,axis_switch_cfg_1->BaseAddress);
		if(status != XST_SUCCESS)
		{
			printf("Error initializing axis switch 1\n");
		}
	}
	axis_switch_cfg_2 = XAxisScr_LookupConfig(XPAR_AXIS_SWITCH_2_DEVICE_ID);
	if(axis_switch_cfg_2)
	{
		int status = XAxisScr_CfgInitialize(&axis_switch_2,axis_switch_cfg_2,axis_switch_cfg_2->BaseAddress);
		if(status != XST_SUCCESS)
		{
			printf("Error initializing axis switch 2\n");
		}
	}
}

void initPeripherals()
{
	init_yolo_conv();
	init_yolo_max_pool();
	init_yolo_yolo();
	init_yolo_acc();
	init_yolo_upsamp();
	init_axi_dma();
	init_axis_switch();
}

void set_yolo_conv(u32 output_ch,u32 input_ch,u32 fold_output_ch,u32 fold_input_ch,
		 	 	   u32 input_h,u32 input_w,u32 real_input_h,
				   u32 fold_win_area)
{
    XYolo_conv_top_Set_output_ch_V(&yolo_conv_top,output_ch);
    XYolo_conv_top_Set_input_ch_V(&yolo_conv_top,input_ch);
    XYolo_conv_top_Set_fold_output_ch_V(&yolo_conv_top,fold_output_ch);
    XYolo_conv_top_Set_fold_input_ch_V(&yolo_conv_top,fold_input_ch);
    XYolo_conv_top_Set_input_h_V(&yolo_conv_top,input_h);
    XYolo_conv_top_Set_input_w_V(&yolo_conv_top,input_w);
    XYolo_conv_top_Set_real_input_h_V(&yolo_conv_top,real_input_h);
    XYolo_conv_top_Set_fold_win_area_V(&yolo_conv_top,fold_win_area);
}

void set_yolo_max_pool(u32 output_h,u32 output_w,
		               u32 input_h,u32 input_w,
					   u32 input_fold_ch,
					   u32 stride)
{
    XYolo_max_pool_top_Set_output_h_V(&yolo_max_pool_top,output_h);
    XYolo_max_pool_top_Set_output_w_V(&yolo_max_pool_top,output_w);
    XYolo_max_pool_top_Set_input_h_V(&yolo_max_pool_top,input_h);
    XYolo_max_pool_top_Set_input_w_V(&yolo_max_pool_top,input_w);
    XYolo_max_pool_top_Set_input_fold_ch_V(&yolo_max_pool_top,input_fold_ch);
    XYolo_max_pool_top_Set_stride_V(&yolo_max_pool_top,stride);
}

void set_yolo_yolo(u32 activate_en,u32 input_h,u32 input_w)
{
    XYolo_yolo_top_Set_activate_en_V(&yolo_yolo_top,activate_en);
    XYolo_yolo_top_Set_input_h_V(&yolo_yolo_top,input_h);
    XYolo_yolo_top_Set_input_w_V(&yolo_yolo_top,input_w);
}

void set_yolo_acc(u32 input_h,u32 input_w,u32 fold_input_ch,u32 leaky,u32 bias_en)
{
    XYolo_acc_top_Set_input_h_V(&yolo_acc_top,input_h);
    XYolo_acc_top_Set_input_w_V(&yolo_acc_top,input_w);
    XYolo_acc_top_Set_fold_input_ch_V(&yolo_acc_top,fold_input_ch);
	XYolo_acc_top_Set_leaky_V(&yolo_acc_top,leaky);
	XYolo_acc_top_Set_bias_en_V(&yolo_acc_top,bias_en);
}

void set_axis_switch(u8 switch_0_s,u8 switch_0_m,
		             u8 switch_1_s,u8 switch_1_m,
					 u8 switch_2_s,u8 switch_2_m)
{
    XAxisScr_RegUpdateDisable(&axis_switch_0);
    XAxisScr_RegUpdateDisable(&axis_switch_1);
    XAxisScr_RegUpdateDisable(&axis_switch_2);

    XAxisScr_MiPortDisableAll(&axis_switch_0);
    XAxisScr_MiPortDisableAll(&axis_switch_1);
    XAxisScr_MiPortDisableAll(&axis_switch_2);

    XAxisScr_MiPortEnable(&axis_switch_0,switch_0_m,switch_0_s);
    XAxisScr_MiPortEnable(&axis_switch_1,switch_1_m,switch_1_s);
    XAxisScr_MiPortEnable(&axis_switch_2,switch_2_m,switch_2_s);

    XAxisScr_RegUpdateEnable(&axis_switch_0);
    XAxisScr_RegUpdateEnable(&axis_switch_1);
    XAxisScr_RegUpdateEnable(&axis_switch_2);
}


short layer_output_hls[26*26*256];
 u32 activate_en[8]={0xfffffff3,0xffffffff,0xfe7fffff,0xffffffff,0xffffffff,0xffffcfff,0xffffffff,0x7fffffff};
int main()
{
	XTime tEnd, tStart;
	int time_used_hw;

	//initialise IPs
    initPeripherals();


    	XTime_GetTime(&tStart);

        int output_fold_factor = 8;
        int input_fold_factor = 8;
        int input_size = 26*26*32;
        int output_size = 26*26*32;

        for(int i=0; i<output_fold_factor; i++)
        {
            short * acc_input_buff = (short *)calloc(input_size,sizeof(short));
            short * acc_output_buff = (short *)calloc(input_size,sizeof(short));
        for(int j=0; j<input_fold_factor; j++)
        {


        if(j==input_fold_factor-1)
    	{
        	set_yolo_conv(32,32,8,8,26+2,26+2,26+2,3);
        	set_yolo_acc(26,26,8,0,1);
        	set_yolo_yolo(activate_en[i],26,26);
            set_axis_switch(0,0,0,2,2,0);
            XYolo_conv_top_Start(&yolo_conv_top);
            XYolo_acc_top_Start(&yolo_acc_top);
            XYolo_yolo_top_Start(&yolo_yolo_top);
    	}
        else
        {
        	set_yolo_conv(32,32,8,8,26+2,26+2,26+2,3);
        	set_yolo_acc(26,26,8,0,0);
        	set_axis_switch(0,0,0,0,0,0);
        	XYolo_conv_top_Start(&yolo_conv_top);
        	XYolo_acc_top_Start(&yolo_acc_top);
        }

        //weight_stream
        Xil_DCacheFlushRange((u32)(&kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short));
        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&(kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DMA_TO_DEVICE));


        if(j==input_fold_factor-1)
        {
            Xil_DCacheFlushRange((u32)(&kernel_bias_fp_bits_pad[32*i]),32*sizeof(short));
            XAxiDma_SimpleTransfer(&axiDMA_1,(u32)&(kernel_bias_fp_bits_pad[32*i]),32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
            while(XAxiDma_Busy(&axiDMA_1,XAXIDMA_DMA_TO_DEVICE));
            Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
            Xil_DCacheFlushRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
            Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
            XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
            XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
            XAxiDma_SimpleTransfer(&axiDMA_0,(u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
            while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
            Xil_DCacheInvalidateRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
            while(!XYolo_yolo_top_IsDone(&yolo_yolo_top));
        }
        else
        {
            Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
            Xil_DCacheFlushRange((u32)acc_output_buff,input_size*sizeof(short));
            Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
            XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
            XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
            XAxiDma_SimpleTransfer(&axiDMA_0,(u32)acc_output_buff,input_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
            while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
            Xil_DCacheInvalidateRange((u32)acc_output_buff,input_size*sizeof(short));
            while(!XYolo_acc_top_IsDone(&yolo_acc_top));
        }


        memcpy(acc_input_buff,acc_output_buff,input_size*sizeof(short));


        }

        free(acc_input_buff);
        free(acc_output_buff);
        }

        XTime_GetTime(&tEnd);


	printf("Calculation complete\n");
    time_used_hw = 1.0*((tEnd-tStart)*1000000)/(COUNTS_PER_SECOND);
    xil_printf("***%d(us)***\n\r", time_used_hw);
int count =0;
	for(int idx=0;idx<output_size*output_fold_factor;idx++)
	{
		short ref = layer_output_ref[idx];
		short get = layer_output_hls[idx];

//		if(abs(ref-get)<64)
//		{
//			continue;
//		}
//		else
//		{count++;
//		//printf("Recv[%d]=%hd,Ref=%hd\n",idx,get,ref);
//		}
		printf("%hd,\n",get);

	}

	printf("End,%d\n",count);
    return 0;
}

//void run_group_0()
//{
//XTime_GetTime(&tStart);
//
////set IP parameters
//set_yolo_conv(16,3,4,1,416+2,416+2,416+2,3);
//set_yolo_acc(416,416,4,1,1);
//set_yolo_max_pool(208,208,416,416,4,2);
//set_axis_switch(0,1,1,0);
//
////start IP
//XYolo_conv_top_Start(&yolo_conv_top);
//XYolo_max_pool_top_Start(&yolo_max_pool_top);
//XYolo_acc_top_Start(&yolo_acc_top);
//
////weight_stream
//Xil_DCacheFlushRange((u32)kernel_weight_fp_bits_pad,12*3*16*sizeof(short));
//XAxiDma_SimpleTransfer(&axiDMA_0,(u32)kernel_weight_fp_bits_pad,12*3*16*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DMA_TO_DEVICE));
//Xil_DCacheFlushRange((u32)kernel_bias_fp_bits_pad,16*sizeof(short));
//XAxiDma_SimpleTransfer(&axiDMA_1,(u32)kernel_bias_fp_bits_pad,16*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//while(XAxiDma_Busy(&axiDMA_1,XAXIDMA_DMA_TO_DEVICE));
//
////data stream
//int output_fold_factor = 1;
//int input_size = 416*416*4;
//int mid_size = 416*416*16;
//int output_size = 208*208*16;
//short * acc_input_buff = (short *)calloc(mid_size,sizeof(short));
//
//Xil_DCacheFlushRange((u32)layer_input,input_size*sizeof(short));
//Xil_DCacheFlushRange((u32)layer_output_hls,output_size*sizeof(short));
//Xil_DCacheFlushRange((u32)acc_input_buff,mid_size*sizeof(short));
////printf("Sending data to IP core slave\n");
//XAxiDma_SimpleTransfer(&axiDMA_0,(u32)layer_input,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//
//XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,mid_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//XAxiDma_SimpleTransfer(&axiDMA_0,(u32)layer_output_hls,output_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
////printf("Get data\n");
//Xil_DCacheInvalidateRange((u32)layer_output_hls,output_size*sizeof(short));
//while(!XYolo_max_pool_top_IsDone(&yolo_max_pool_top));
//
//free(acc_input_buff);
//XTime_GetTime(&tEnd);
//}

//void run_group_1()
//{
//XTime_GetTime(&tStart);
//
// //set IP parameters
// set_yolo_conv(32,16,8,4,208+2,208+2,208+2,3);
// set_yolo_acc(208,208,8,1,1);
// set_yolo_max_pool(104,104,208,208,8,2);
// set_axis_switch(0,1,1,0);
//
// //start IP
// XYolo_conv_top_Start(&yolo_conv_top);
// XYolo_max_pool_top_Start(&yolo_max_pool_top);
// XYolo_acc_top_Start(&yolo_acc_top);
//
// //weight_stream
// Xil_DCacheFlushRange((u32)kernel_weight_fp_bits_pad,12*16*32*sizeof(short));
// XAxiDma_SimpleTransfer(&axiDMA_0,(u32)kernel_weight_fp_bits_pad,12*16*32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
// while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DMA_TO_DEVICE));
// Xil_DCacheFlushRange((u32)kernel_bias_fp_bits_pad,32*sizeof(short));
// XAxiDma_SimpleTransfer(&axiDMA_1,(u32)kernel_bias_fp_bits_pad,32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
// while(XAxiDma_Busy(&axiDMA_1,XAXIDMA_DMA_TO_DEVICE));
//
// //data stream
// int output_fold_factor = 1;
// int input_size = 208*208*16;
// int mid_size = 208*208*32;
// int output_size = 104*104*32;
// short * acc_input_buff = (short *)calloc(mid_size,sizeof(short));
//
// Xil_DCacheFlushRange((u32)layer_input,input_size*sizeof(short));
// Xil_DCacheFlushRange((u32)layer_output_hls,output_size*sizeof(short));
// Xil_DCacheFlushRange((u32)acc_input_buff,mid_size*sizeof(short));
// //printf("Sending data to IP core slave\n");
// XAxiDma_SimpleTransfer(&axiDMA_0,(u32)layer_input,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//
// XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,mid_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//	XAxiDma_SimpleTransfer(&axiDMA_0,(u32)layer_output_hls,output_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//	while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//	//printf("Get data\n");
//	Xil_DCacheInvalidateRange((u32)layer_output_hls,output_size*sizeof(short));
//	while(!XYolo_max_pool_top_IsDone(&yolo_max_pool_top));
//
//	free(acc_input_buff);
// XTime_GetTime(&tEnd);
//}

//void run_group_2()
//{
//	XTime_GetTime(&tStart);
//	int output_fold_factor = 2;
//	int input_size = 104*104*32;
//	int mid_size = 104*104*64/output_fold_factor;
//	int output_size = 52*52*64/output_fold_factor;
//
//	for(int i=0; i<output_fold_factor; i++)
//	{
//	//set IP parameters
//	set_yolo_conv(32,32,8,8,104+2,104+2,104+2,3);
//	set_yolo_acc(104,104,8,1,1);
//	set_yolo_max_pool(52,52,104,104,8,2);
//	set_axis_switch(0,1,1,0);
//
//	//start IP
//	XYolo_conv_top_Start(&yolo_conv_top);
//	XYolo_max_pool_top_Start(&yolo_max_pool_top);
//	XYolo_acc_top_Start(&yolo_acc_top);
//
//	//weight_stream
//	Xil_DCacheFlushRange((u32)(&kernel_weight_fp_bits_pad[12*32*32*i]),12*32*32*sizeof(short));
//	XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&(kernel_weight_fp_bits_pad[12*32*32*i]),12*32*32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//	while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DMA_TO_DEVICE));
//	Xil_DCacheFlushRange((u32)(&kernel_bias_fp_bits_pad[32*i]),32*sizeof(short));
//	XAxiDma_SimpleTransfer(&axiDMA_1,(u32)&(kernel_bias_fp_bits_pad[32*i]),32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//	while(XAxiDma_Busy(&axiDMA_1,XAXIDMA_DMA_TO_DEVICE));
//
//	//data stream
//
//	short * acc_input_buff = (short *)calloc(mid_size,sizeof(short));
//
//	Xil_DCacheFlushRange((u32)layer_input,input_size*sizeof(short));
//	Xil_DCacheFlushRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//	Xil_DCacheFlushRange((u32)acc_input_buff,mid_size*sizeof(short));
//	//printf("Sending data to IP core slave\n");
//	XAxiDma_SimpleTransfer(&axiDMA_0,(u32)layer_input,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//
//	XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,mid_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//	XAxiDma_SimpleTransfer(&axiDMA_0,(u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//	while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//	//printf("Get data\n");
//	Xil_DCacheInvalidateRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//	while(!XYolo_max_pool_top_IsDone(&yolo_max_pool_top));
//
//	free(acc_input_buff);
//	}
//	XTime_GetTime(&tEnd);
//}

//void run_group_3()
//{
//    XTime_GetTime(&tStart);
//
//    int output_fold_factor = 4;
//    int input_fold_factor = 2;
//    int input_size = 52*52*32;
//    int output_size = 26*26*32;
//
//    for(int i=0; i<output_fold_factor; i++)
//    {
//        short * acc_input_buff = (short *)calloc(input_size,sizeof(short));
//        short * acc_output_buff = (short *)calloc(input_size,sizeof(short));
//    for(int j=0; j<input_fold_factor; j++)
//    {
//
//
//    if(j==input_fold_factor-1)
//	{
//    	set_yolo_conv(32,32,8,8,52+2,52+2,52+2,3);
//    	set_yolo_acc(52,52,8,1,1);
//        set_yolo_max_pool(26,26,52,52,8,2);
//        set_axis_switch(0,1,1,0);
//        XYolo_conv_top_Start(&yolo_conv_top);
//        XYolo_acc_top_Start(&yolo_acc_top);
//        XYolo_max_pool_top_Start(&yolo_max_pool_top);
//	}
//    else
//    {
//    	set_yolo_conv(32,32,8,8,52+2,52+2,52+2,3);
//    	set_yolo_acc(52,52,8,0,0);
//    	set_axis_switch(0,0,0,0);
//    	XYolo_conv_top_Start(&yolo_conv_top);
//    	XYolo_acc_top_Start(&yolo_acc_top);
//    }
//
//    //weight_stream
//    Xil_DCacheFlushRange((u32)(&kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short));
//    XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&(kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//    while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DMA_TO_DEVICE));
//
//
//    if(j==input_fold_factor-1)
//    {
//        Xil_DCacheFlushRange((u32)(&kernel_bias_fp_bits_pad[32*i]),32*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)&(kernel_bias_fp_bits_pad[32*i]),32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        while(XAxiDma_Busy(&axiDMA_1,XAXIDMA_DMA_TO_DEVICE));
//        Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//        Xil_DCacheInvalidateRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//        while(!XYolo_max_pool_top_IsDone(&yolo_max_pool_top));
//    }
//    else
//    {
//        Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_output_buff,input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)acc_output_buff,input_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//        Xil_DCacheInvalidateRange((u32)acc_output_buff,input_size*sizeof(short));
//        while(!XYolo_acc_top_IsDone(&yolo_acc_top));
//    }
//
//
//    memcpy(acc_input_buff,acc_output_buff,input_size*sizeof(short));
//
//
//    }
//
//    free(acc_input_buff);
//    free(acc_output_buff);
//    }
//
//    XTime_GetTime(&tEnd);
//}


//void run_group_4()
//{
//	XTime_GetTime(&tStart);
//
//    int output_fold_factor = 8;
//    int input_fold_factor = 4;
//    int input_size = 26*26*32;
//    int output_size = 13*13*32;
//
//    for(int i=0; i<output_fold_factor; i++)
//    {
//        short * acc_input_buff = (short *)calloc(input_size,sizeof(short));
//        short * acc_output_buff = (short *)calloc(input_size,sizeof(short));
//    for(int j=0; j<input_fold_factor; j++)
//    {
//
//
//    if(j==input_fold_factor-1)
//	{
//    	set_yolo_conv(32,32,8,8,26+2,26+2,26+2,3);
//    	set_yolo_acc(26,26,8,1,1);
//        set_yolo_max_pool(13,13,26,26,8,2);
//        set_axis_switch(0,1,1,0);
//        XYolo_conv_top_Start(&yolo_conv_top);
//        XYolo_acc_top_Start(&yolo_acc_top);
//        XYolo_max_pool_top_Start(&yolo_max_pool_top);
//	}
//    else
//    {
//    	set_yolo_conv(32,32,8,8,26+2,26+2,26+2,3);
//    	set_yolo_acc(26,26,8,0,0);
//    	set_axis_switch(0,0,0,0);
//    	XYolo_conv_top_Start(&yolo_conv_top);
//    	XYolo_acc_top_Start(&yolo_acc_top);
//    }
//
//    //weight_stream
//    Xil_DCacheFlushRange((u32)(&kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short));
//    XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&(kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//    while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DMA_TO_DEVICE));
//
//
//    if(j==input_fold_factor-1)
//    {
//        Xil_DCacheFlushRange((u32)(&kernel_bias_fp_bits_pad[32*i]),32*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)&(kernel_bias_fp_bits_pad[32*i]),32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        while(XAxiDma_Busy(&axiDMA_1,XAXIDMA_DMA_TO_DEVICE));
//        Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//        Xil_DCacheInvalidateRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//        while(!XYolo_max_pool_top_IsDone(&yolo_max_pool_top));
//    }
//    else
//    {
//        Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_output_buff,input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)acc_output_buff,input_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//        Xil_DCacheInvalidateRange((u32)acc_output_buff,input_size*sizeof(short));
//        while(!XYolo_acc_top_IsDone(&yolo_acc_top));
//    }
//
//
//    memcpy(acc_input_buff,acc_output_buff,input_size*sizeof(short));
//
//
//    }
//
//    free(acc_input_buff);
//    free(acc_output_buff);
//    }
//
//    XTime_GetTime(&tEnd);
//}

//void run_group_4_0()
//{
//   	XTime_GetTime(&tStart);
//
//        int output_fold_factor = 8;
//        int input_fold_factor = 4;
//        int input_size = 26*26*32;
//        int output_size = 26*26*32;
//
//        for(int i=0; i<output_fold_factor; i++)
//        {
//            short * acc_input_buff = (short *)calloc(input_size,sizeof(short));
//            short * acc_output_buff = (short *)calloc(input_size,sizeof(short));
//        for(int j=0; j<input_fold_factor; j++)
//        {
//
//
//        if(j==input_fold_factor-1)
//    	{
//        	set_yolo_conv(32,32,8,8,26+2,26+2,26+2,3);
//        	set_yolo_acc(26,26,8,1,1);
//            set_axis_switch(0,0,0,0,0,0);
//            XYolo_conv_top_Start(&yolo_conv_top);
//            XYolo_acc_top_Start(&yolo_acc_top);
//    	}
//        else
//        {
//        	set_yolo_conv(32,32,8,8,26+2,26+2,26+2,3);
//        	set_yolo_acc(26,26,8,0,0);
//        	set_axis_switch(0,0,0,0,0,0);
//        	XYolo_conv_top_Start(&yolo_conv_top);
//        	XYolo_acc_top_Start(&yolo_acc_top);
//        }
//
//        //weight_stream
//        Xil_DCacheFlushRange((u32)(&kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&(kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DMA_TO_DEVICE));
//
//
//        if(j==input_fold_factor-1)
//        {
//            Xil_DCacheFlushRange((u32)(&kernel_bias_fp_bits_pad[32*i]),32*sizeof(short));
//            XAxiDma_SimpleTransfer(&axiDMA_1,(u32)&(kernel_bias_fp_bits_pad[32*i]),32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//            while(XAxiDma_Busy(&axiDMA_1,XAXIDMA_DMA_TO_DEVICE));
//            Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//            Xil_DCacheFlushRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//            Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//            XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//            XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//            XAxiDma_SimpleTransfer(&axiDMA_0,(u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//            while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//            Xil_DCacheInvalidateRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//            while(!XYolo_acc_top_IsDone(&yolo_acc_top));
//        }
//        else
//        {
//            Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//            Xil_DCacheFlushRange((u32)acc_output_buff,input_size*sizeof(short));
//            Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//            XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//            XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//            XAxiDma_SimpleTransfer(&axiDMA_0,(u32)acc_output_buff,input_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//            while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//            Xil_DCacheInvalidateRange((u32)acc_output_buff,input_size*sizeof(short));
//            while(!XYolo_acc_top_IsDone(&yolo_acc_top));
//        }
//
//
//        memcpy(acc_input_buff,acc_output_buff,input_size*sizeof(short));
//
//
//        }
//
//        free(acc_input_buff);
//        free(acc_output_buff);
//        }
//
//        XTime_GetTime(&tEnd);
//}

//void run_group_4_1()
//{
//	XTime_GetTime(&tStart);
//
//    int output_fold_factor = 8;
//    int input_fold_factor = 8;
//    int input_size = 26*26*32;
//    int output_size = 13*13*32;
//
//    for(int i=0; i<output_fold_factor; i++)
//    {
//    	set_yolo_max_pool(13,13,26,26,8,2);
//        set_axis_switch(0,1,1,1,1,0);
//        XYolo_max_pool_top_Start(&yolo_max_pool_top);
//
//
//        Xil_DCacheFlushRange((u32)(&layer_input[input_size*i]),input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*i],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//        Xil_DCacheInvalidateRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//        while(!XYolo_max_pool_top_IsDone(&yolo_max_pool_top));
//    }
//
//    XTime_GetTime(&tEnd);
//
//}

//void run_group_5()
//{
//	XTime_GetTime(&tStart);
//
//    int output_fold_factor = 16;
//    int input_fold_factor = 8;
//    int input_size = 13*13*32;
//    int output_size = 13*13*32;
//
//    for(int i=0; i<output_fold_factor; i++)
//    {
//        short * acc_input_buff = (short *)calloc(input_size,sizeof(short));
//        short * acc_output_buff = (short *)calloc(input_size,sizeof(short));
//    for(int j=0; j<input_fold_factor; j++)
//    {
//
//
//    if(j==input_fold_factor-1)
//	{
//    	set_yolo_conv(32,32,8,8,13+2,13+2,13+2,3);
//    	set_yolo_acc(13,13,8,1,1);
//        set_yolo_max_pool(13+1,13+1,13,13,8,1);
//        set_axis_switch(0,1,1,0);
//        XYolo_conv_top_Start(&yolo_conv_top);
//        XYolo_acc_top_Start(&yolo_acc_top);
//        XYolo_max_pool_top_Start(&yolo_max_pool_top);
//	}
//    else
//    {
//    	set_yolo_conv(32,32,8,8,13+2,13+2,13+2,3);
//    	set_yolo_acc(13,13,8,0,0);
//    	set_axis_switch(0,0,0,0);
//    	XYolo_conv_top_Start(&yolo_conv_top);
//    	XYolo_acc_top_Start(&yolo_acc_top);
//    }
//
//    //weight_stream
//    Xil_DCacheFlushRange((u32)(&kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short));
//    XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&(kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//    while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DMA_TO_DEVICE));
//
//
//    if(j==input_fold_factor-1)
//    {
//        Xil_DCacheFlushRange((u32)(&kernel_bias_fp_bits_pad[32*i]),32*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)&(kernel_bias_fp_bits_pad[32*i]),32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        while(XAxiDma_Busy(&axiDMA_1,XAXIDMA_DMA_TO_DEVICE));
//        Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//        Xil_DCacheInvalidateRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//        while(!XYolo_max_pool_top_IsDone(&yolo_max_pool_top));
//    }
//    else
//    {
//        Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_output_buff,input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)acc_output_buff,input_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//        Xil_DCacheInvalidateRange((u32)acc_output_buff,input_size*sizeof(short));
//        while(!XYolo_acc_top_IsDone(&yolo_acc_top));
//    }
//
//
//    memcpy(acc_input_buff,acc_output_buff,input_size*sizeof(short));
//
//
//    }
//
//    free(acc_input_buff);
//    free(acc_output_buff);
//    }
//
//    XTime_GetTime(&tEnd);
//}

//void run_group_6()
//{
//   	XTime_GetTime(&tStart);
//
//        int output_fold_factor = 32;
//        int input_fold_factor = 16;
//        int input_size = 13*13*32;
//        int output_size = 13*13*32;
//
//        for(int i=0; i<output_fold_factor; i++)
//        {
//            short * acc_input_buff = (short *)calloc(input_size,sizeof(short));
//            short * acc_output_buff = (short *)calloc(input_size,sizeof(short));
//        for(int j=0; j<input_fold_factor; j++)
//        {
//
//
//        if(j==input_fold_factor-1)
//    	{
//        	set_yolo_conv(32,32,8,8,13+2,13+2,13+2,3);
//        	set_yolo_acc(13,13,8,1,1);
//            set_axis_switch(0,0,0,0);
//            XYolo_conv_top_Start(&yolo_conv_top);
//            XYolo_acc_top_Start(&yolo_acc_top);
//    	}
//        else
//        {
//        	set_yolo_conv(32,32,8,8,13+2,13+2,13+2,3);
//        	set_yolo_acc(13,13,8,0,0);
//        	set_axis_switch(0,0,0,0);
//        	XYolo_conv_top_Start(&yolo_conv_top);
//        	XYolo_acc_top_Start(&yolo_acc_top);
//        }
//
//        //weight_stream
//        Xil_DCacheFlushRange((u32)(&kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&(kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DMA_TO_DEVICE));
//
//
//        if(j==input_fold_factor-1)
//        {
//            Xil_DCacheFlushRange((u32)(&kernel_bias_fp_bits_pad[32*i]),32*sizeof(short));
//            XAxiDma_SimpleTransfer(&axiDMA_1,(u32)&(kernel_bias_fp_bits_pad[32*i]),32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//            while(XAxiDma_Busy(&axiDMA_1,XAXIDMA_DMA_TO_DEVICE));
//            Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//            Xil_DCacheFlushRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//            Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//            XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//            XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//            XAxiDma_SimpleTransfer(&axiDMA_0,(u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//            while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//            Xil_DCacheInvalidateRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//            while(!XYolo_acc_top_IsDone(&yolo_acc_top));
//        }
//        else
//        {
//            Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//            Xil_DCacheFlushRange((u32)acc_output_buff,input_size*sizeof(short));
//            Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//            XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//            XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//            XAxiDma_SimpleTransfer(&axiDMA_0,(u32)acc_output_buff,input_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//            while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//            Xil_DCacheInvalidateRange((u32)acc_output_buff,input_size*sizeof(short));
//            while(!XYolo_acc_top_IsDone(&yolo_acc_top));
//        }
//
//
//        memcpy(acc_input_buff,acc_output_buff,input_size*sizeof(short));
//
//
//        }
//
//        free(acc_input_buff);
//        free(acc_output_buff);
//        }
//
//        XTime_GetTime(&tEnd);
//}

//void run_group_7()
//{
//	XTime_GetTime(&tStart);
//
//    int output_fold_factor = 8;
//    int input_fold_factor = 32;
//    int input_size = 13*13*32;
//    int output_size = 13*13*32;
//
//    for(int i=0; i<output_fold_factor; i++)
//    {
//        short * acc_input_buff = (short *)calloc(input_size,sizeof(short));
//        short * acc_output_buff = (short *)calloc(input_size,sizeof(short));
//    for(int j=0; j<input_fold_factor; j++)
//    {
//
//
//    if(j==input_fold_factor-1)
//	{
//    	set_yolo_conv(32,32,8,8,13+2,13+2,13+2,3);
//    	set_yolo_acc(13,13,8,1,1);
//        set_axis_switch(0,0,0,0);
//        XYolo_conv_top_Start(&yolo_conv_top);
//        XYolo_acc_top_Start(&yolo_acc_top);
//	}
//    else
//    {
//    	set_yolo_conv(32,32,8,8,13+2,13+2,13+2,3);
//    	set_yolo_acc(13,13,8,0,0);
//    	set_axis_switch(0,0,0,0);
//    	XYolo_conv_top_Start(&yolo_conv_top);
//    	XYolo_acc_top_Start(&yolo_acc_top);
//    }
//
//    //weight_stream
//    Xil_DCacheFlushRange((u32)(&kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short));
//    XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&(kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//    while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DMA_TO_DEVICE));
//
//
//    if(j==input_fold_factor-1)
//    {
//        Xil_DCacheFlushRange((u32)(&kernel_bias_fp_bits_pad[32*i]),32*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)&(kernel_bias_fp_bits_pad[32*i]),32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        while(XAxiDma_Busy(&axiDMA_1,XAXIDMA_DMA_TO_DEVICE));
//        Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//        Xil_DCacheInvalidateRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//        while(!XYolo_acc_top_IsDone(&yolo_acc_top));
//    }
//    else
//    {
//        Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_output_buff,input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)acc_output_buff,input_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//        Xil_DCacheInvalidateRange((u32)acc_output_buff,input_size*sizeof(short));
//        while(!XYolo_acc_top_IsDone(&yolo_acc_top));
//    }
//
//
//    memcpy(acc_input_buff,acc_output_buff,input_size*sizeof(short));
//
//
//    }
//
//    free(acc_input_buff);
//    free(acc_output_buff);
//    }
//
//    XTime_GetTime(&tEnd);
//}

//void run_group_8()
//{
//	XTime_GetTime(&tStart);
//
//    int output_fold_factor = 16;
//    int input_fold_factor = 8;
//    int input_size = 13*13*32;
//    int output_size = 13*13*32;
//
//    for(int i=0; i<output_fold_factor; i++)
//    {
//        short * acc_input_buff = (short *)calloc(input_size,sizeof(short));
//        short * acc_output_buff = (short *)calloc(input_size,sizeof(short));
//    for(int j=0; j<input_fold_factor; j++)
//    {
//
//
//    if(j==input_fold_factor-1)
//	{
//    	set_yolo_conv(32,32,8,8,13+2,13+2,13+2,3);
//    	set_yolo_acc(13,13,8,1,1);
//        set_axis_switch(0,0,0,0);
//        XYolo_conv_top_Start(&yolo_conv_top);
//        XYolo_acc_top_Start(&yolo_acc_top);
//	}
//    else
//    {
//    	set_yolo_conv(32,32,8,8,13+2,13+2,13+2,3);
//    	set_yolo_acc(13,13,8,0,0);
//    	set_axis_switch(0,0,0,0);
//    	XYolo_conv_top_Start(&yolo_conv_top);
//    	XYolo_acc_top_Start(&yolo_acc_top);
//    }
//
//    //weight_stream
//    Xil_DCacheFlushRange((u32)(&kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short));
//    XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&(kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//    while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DMA_TO_DEVICE));
//
//
//    if(j==input_fold_factor-1)
//    {
//        Xil_DCacheFlushRange((u32)(&kernel_bias_fp_bits_pad[32*i]),32*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)&(kernel_bias_fp_bits_pad[32*i]),32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        while(XAxiDma_Busy(&axiDMA_1,XAXIDMA_DMA_TO_DEVICE));
//        Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//        Xil_DCacheInvalidateRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//        while(!XYolo_acc_top_IsDone(&yolo_acc_top));
//    }
//    else
//    {
//        Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_output_buff,input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)acc_output_buff,input_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//        Xil_DCacheInvalidateRange((u32)acc_output_buff,input_size*sizeof(short));
//        while(!XYolo_acc_top_IsDone(&yolo_acc_top));
//    }
//
//
//    memcpy(acc_input_buff,acc_output_buff,input_size*sizeof(short));
//
//
//    }
//
//    free(acc_input_buff);
//    free(acc_output_buff);
//    }
//
//    XTime_GetTime(&tEnd);
//}

//void run_group_9()
//{
//	XTime_GetTime(&tStart);
//
//    int output_fold_factor = 8;
//    int input_fold_factor = 16;
//    int input_size = 13*13*32;
//    int output_size = 13*13*32;
//
//    for(int i=0; i<output_fold_factor; i++)
//    {
//        short * acc_input_buff = (short *)calloc(input_size,sizeof(short));
//        short * acc_output_buff = (short *)calloc(input_size,sizeof(short));
//    for(int j=0; j<input_fold_factor; j++)
//    {
//
//
//    if(j==input_fold_factor-1)
//	{
//    	set_yolo_conv(32,32,8,8,13+2,13+2,13+2,3);
//    	set_yolo_acc(13,13,8,0,1);
//    	set_yolo_yolo(activate_en[i],13,13);
//        set_axis_switch(0,2,2,0);
//        XYolo_conv_top_Start(&yolo_conv_top);
//        XYolo_acc_top_Start(&yolo_acc_top);
//        XYolo_yolo_top_Start(&yolo_yolo_top);
//	}
//    else
//    {
//    	set_yolo_conv(32,32,8,8,13+2,13+2,13+2,3);
//    	set_yolo_acc(13,13,8,0,0);
//    	set_axis_switch(0,0,0,0);
//    	XYolo_conv_top_Start(&yolo_conv_top);
//    	XYolo_acc_top_Start(&yolo_acc_top);
//    }
//
//    //weight_stream
//    Xil_DCacheFlushRange((u32)(&kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short));
//    XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&(kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//    while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DMA_TO_DEVICE));
//
//
//    if(j==input_fold_factor-1)
//    {
//        Xil_DCacheFlushRange((u32)(&kernel_bias_fp_bits_pad[32*i]),32*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)&(kernel_bias_fp_bits_pad[32*i]),32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        while(XAxiDma_Busy(&axiDMA_1,XAXIDMA_DMA_TO_DEVICE));
//        Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//        Xil_DCacheInvalidateRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//        while(!XYolo_yolo_top_IsDone(&yolo_yolo_top));
//    }
//    else
//    {
//        Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_output_buff,input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)acc_output_buff,input_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//        Xil_DCacheInvalidateRange((u32)acc_output_buff,input_size*sizeof(short));
//        while(!XYolo_acc_top_IsDone(&yolo_acc_top));
//    }
//
//
//    memcpy(acc_input_buff,acc_output_buff,input_size*sizeof(short));
//
//
//    }
//
//    free(acc_input_buff);
//    free(acc_output_buff);
//    }
//
//    XTime_GetTime(&tEnd);
//
//}

//void run_group_10()
//{
//	XTime_GetTime(&tStart);
//
//    int output_fold_factor = 4;
//    int input_fold_factor = 8;
//    int input_size = 13*13*32;
//    int output_size = 26*26*32;
//
//    for(int i=0; i<output_fold_factor; i++)
//    {
//        short * acc_input_buff = (short *)calloc(input_size,sizeof(short));
//        short * acc_output_buff = (short *)calloc(input_size,sizeof(short));
//    for(int j=0; j<input_fold_factor; j++)
//    {
//
//
//    if(j==input_fold_factor-1)
//	{
//    	set_yolo_conv(32,32,8,8,13+2,13+2,13+2,3);
//    	set_yolo_acc(13,13,8,1,1);
//        set_axis_switch(0,0,0,3,3,0);
//        XYolo_conv_top_Start(&yolo_conv_top);
//        XYolo_acc_top_Start(&yolo_acc_top);
//        XYolo_upsamp_top_Start(&yolo_upsamp_top);
//	}
//    else
//    {
//    	set_yolo_conv(32,32,8,8,13+2,13+2,13+2,3);
//    	set_yolo_acc(13,13,8,0,0);
//    	set_axis_switch(0,0,0,0,0,0);
//    	XYolo_conv_top_Start(&yolo_conv_top);
//    	XYolo_acc_top_Start(&yolo_acc_top);
//    }
//
//    //weight_stream
//    Xil_DCacheFlushRange((u32)(&kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short));
//    XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&(kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//    while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DMA_TO_DEVICE));
//
//
//    if(j==input_fold_factor-1)
//    {
//        Xil_DCacheFlushRange((u32)(&kernel_bias_fp_bits_pad[32*i]),32*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)&(kernel_bias_fp_bits_pad[32*i]),32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        while(XAxiDma_Busy(&axiDMA_1,XAXIDMA_DMA_TO_DEVICE));
//        Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//        Xil_DCacheInvalidateRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//        while(!XYolo_upsamp_top_IsDone(&yolo_upsamp_top));
//    }
//    else
//    {
//        Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_output_buff,input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)acc_output_buff,input_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//        Xil_DCacheInvalidateRange((u32)acc_output_buff,input_size*sizeof(short));
//        while(!XYolo_acc_top_IsDone(&yolo_acc_top));
//    }
//
//
//    memcpy(acc_input_buff,acc_output_buff,input_size*sizeof(short));
//
//
//    }
//
//    free(acc_input_buff);
//    free(acc_output_buff);
//    }
//
//    XTime_GetTime(&tEnd);
//
//}

//void run_route_1()
//{
//	XTime_GetTime(&tStart);
//int output_size = 26*26*32;
//int output_fold_factor = 12;
//int output_route_factor_0 = 4;
//int output_route_factor_1 = 8;
//
//memcpy(layer_output_hls,layer_input_0,output_size*output_route_factor_0*sizeof(short));
//memcpy(&layer_output_hls[output_size*output_route_factor_0],layer_input_1,output_size*output_route_factor_1*sizeof(short));
//
//    XTime_GetTime(&tEnd);
//}

//void run_group_11()
//{
//	XTime_GetTime(&tStart);
//
//    int output_fold_factor = 8;
//    int input_fold_factor = 12;
//    int input_size = 26*26*32;
//    int output_size = 26*26*32;
//
//    for(int i=0; i<output_fold_factor; i++)
//    {
//        short * acc_input_buff = (short *)calloc(input_size,sizeof(short));
//        short * acc_output_buff = (short *)calloc(input_size,sizeof(short));
//    for(int j=0; j<input_fold_factor; j++)
//    {
//
//
//    if(j==input_fold_factor-1)
//	{
//    	set_yolo_conv(32,32,8,8,26+2,26+2,26+2,3);
//    	set_yolo_acc(26,26,8,1,1);
//        set_axis_switch(0,0,0,0,0,0);
//        XYolo_conv_top_Start(&yolo_conv_top);
//        XYolo_acc_top_Start(&yolo_acc_top);
//	}
//    else
//    {
//    	set_yolo_conv(32,32,8,8,26+2,26+2,26+2,3);
//    	set_yolo_acc(26,26,8,0,0);
//    	set_axis_switch(0,0,0,0,0,0);
//    	XYolo_conv_top_Start(&yolo_conv_top);
//    	XYolo_acc_top_Start(&yolo_acc_top);
//    }
//
//    //weight_stream
//    Xil_DCacheFlushRange((u32)(&kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short));
//    XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&(kernel_weight_fp_bits_pad[12*32*32*output_fold_factor*j+12*32*32*i]),12*32*32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//    while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DMA_TO_DEVICE));
//
//
//    if(j==input_fold_factor-1)
//    {
//        Xil_DCacheFlushRange((u32)(&kernel_bias_fp_bits_pad[32*i]),32*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)&(kernel_bias_fp_bits_pad[32*i]),32*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        while(XAxiDma_Busy(&axiDMA_1,XAXIDMA_DMA_TO_DEVICE));
//        Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//        Xil_DCacheInvalidateRange((u32)(&layer_output_hls[output_size*i]),output_size*sizeof(short));
//        while(!XYolo_acc_top_IsDone(&yolo_acc_top));
//    }
//    else
//    {
//        Xil_DCacheFlushRange((u32)(&layer_input[input_size*j]),input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_output_buff,input_size*sizeof(short));
//        Xil_DCacheFlushRange((u32)acc_input_buff,input_size*sizeof(short));
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)&layer_input[input_size*j],input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_1,(u32)acc_input_buff,input_size*sizeof(short),XAXIDMA_DMA_TO_DEVICE);
//        XAxiDma_SimpleTransfer(&axiDMA_0,(u32)acc_output_buff,input_size*sizeof(short),XAXIDMA_DEVICE_TO_DMA);
//        while(XAxiDma_Busy(&axiDMA_0,XAXIDMA_DEVICE_TO_DMA));
//        Xil_DCacheInvalidateRange((u32)acc_output_buff,input_size*sizeof(short));
//        while(!XYolo_acc_top_IsDone(&yolo_acc_top));
//    }
//
//
//    memcpy(acc_input_buff,acc_output_buff,input_size*sizeof(short));
//
//
//    }
//
//    free(acc_input_buff);
//    free(acc_output_buff);
//    }
//
//    XTime_GetTime(&tEnd);
//}
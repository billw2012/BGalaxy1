/*
 * Copyright 1993-2010 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */
 

__kernel void DotProduct (__global float4* a, __global float4* b, __global float4* c)
{
    // find position in global arrays
    int iGID = get_global_id(0);

    // process 
    c[iGID] = a[iGID] + b[iGID];
}

/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//确定给定行/列的单元格应该是什么颜色。这不应影响图像,并且应该为新的颜色分配空间。
Color *evaluateOnePixel(Image *image, int row, int col) {
    Color* newColor = malloc(sizeof(Color));
    if (newColor == NULL) {
        fprintf(stderr, "Memory allocation failed in evaluateOnePixel\n");
        exit(-1);
    }

    Color original = image->image[row][col];



    // 取出最低有效位
    if (original.B & 1) {
        newColor->R = 255;
        newColor->G = 255;
        newColor->B = 255;
    } else {
        newColor->R = 0;
        newColor->G = 0;
        newColor->B = 0;
    }

    return newColor;
}

//Given an image, creates a new image extracting the LSB of the B channel.
//给定一张图像，创建一张新图像，提取B通道的最低有效位(LSB)。
Image *steganography(Image *image) {
    int rows = image->rows;
    int cols = image->cols;

    Image* decoded = malloc(sizeof(Image));
    if (decoded == NULL) {
        fprintf(stderr, "Memory allocation failed in steganography\n");
        exit(-1);
    }

    decoded->rows = rows;
    decoded->cols = cols;

    decoded->image = malloc(rows * sizeof(Color*));
    if (decoded->image == NULL) {
        fprintf(stderr, "Memory allocation failed for rows in steganography\n");
        free(decoded);
        exit(-1);
    }

    for (int i = 0; i < rows; i++) {
        decoded->image[i] = malloc(cols * sizeof(Color));
        if (decoded->image[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for cols in steganography\n");
            // 清理之前分配的行
            for (int k = 0; k < i; k++) {
                free(decoded->image[k]);
            }
            free(decoded->image);
            free(decoded);
            exit(-1);
        }

        for (int j = 0; j < cols; j++) {
            Color* pixel = evaluateOnePixel(image, i, j);  // 分配了内存
            decoded->image[i][j] = *pixel;                 // 复制值
            free(pixel);                                   // 🔥 释放内存，防止泄漏
        }
    }

    return decoded;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	// 1 解析命令行参数
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return -1;
    }
	// 2 读取PPM图片
	    Image *original = readData(argv[1]);
    if (original == NULL) {
        fprintf(stderr, "Error reading image.\n");
        return -1;
    }
	// 3 解码像素
	    Image *secret = steganography(original);
    if (secret == NULL) {
        fprintf(stderr, "Error processing image.\n");
        freeImage(original);
        return -1;
    }

	// 4 生成新图像 写入输出
	writeData(secret);
 	// 5 free 内存
	freeImage(original);
    freeImage(secret);
	// 6 return 退出码
	return 0;
}

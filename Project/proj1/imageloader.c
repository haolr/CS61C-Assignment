/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
// 读取 PPM 图片数据，返回 Image 结构体的指针
Image *readData(char *filename) 
{
    //---打开文件
    FILE *fp = fopen(filename, "r");  // "r" 表示以只读模式（read mode）打开文件
    if (!fp) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return NULL;
    }
	//---读取文件格式
	char format[3];
	fscanf(fp,"%2s",format);
	if (strcmp(format, "P3") != 0) {
		fprintf(stderr, "Error: Not a valid P3 PPM file\n");
		fclose(fp);
	return NULL;
    }
	//---读取图像的列、行数
	int cols, rows, maxval;
    fscanf(fp, "%d %d", &cols, &rows);  // 第二行：图像尺寸，格式为 "宽 高"
    fscanf(fp, "%d", &maxval);         // 第三行：最大颜色值，一般是 255
    //---分配 Image 结构体
    Image *img = malloc(sizeof(Image));  // 分配内存，返回 Image*，所以我们要返回指针
    img->rows = rows;
    img->cols = cols;
	//---分配二维像素数组
	img->image = malloc(rows*sizeof(Color*));
	for(int i=0;i<rows;i++) {
		img->image[i] = malloc (sizeof(Color)*cols);
	}
	//---读取像素值并且填充结构体
	for(int i = 0;i < rows ;i++) {
		for(int j = 0;j < cols ;j++) {
			int r,g,b;
			fscanf(fp, "%d %d %d",&r,&g,&b);
			img->image[i][j].R = r;
			img->image[i][j].G = g;
			img->image[i][j].B = b;
		}
	}
    //---关闭文件
    fclose(fp);

    return img;  

}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
    printf("P3\n");
    printf("%d %d\n", image->cols, image->rows);
    printf("255\n");

    for (int i = 0; i < image->rows; i++) {
        for (int j = 0; j < image->cols; j++) {
            Color pixel = image->image[i][j];
            printf("%3d %3d %3d", pixel.R, pixel.G, pixel.B);
			if(j != image->cols -1) printf("   ");
        }
        printf("\n");
    }
}

//Frees an image
void freeImage(Image *image)
{
    for (int r = 0; r < image->rows; r++) {
        free(image->image[r]);
    }
	free(image->image);
	free(image);
}
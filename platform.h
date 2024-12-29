/****************************************************************************
** MIT License
**
** Author	: xiaofeng.zhu
** Support	: zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#ifndef sss_PLATFORM_H
#define sss_PLATFORM_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L && !defined(_MSC_VER)
#define USE_VLA 1
#else
#define USE_VLA 0
#endif

#if USE_VLA
#define NEW_ARRAY(type, array, size) type array[size];
#define NEW_ARRAY_2D(type, array, rows, cols) type array[rows][cols];
#define NEW_ARRAY_3D(type, array, dim1, dim2, dim3) type array[dim1][dim2][dim3];
#define DELETE_ARRAY(array) (void)0;
#define DELETE_ARRAY_2D(array, rows) (void)0;
#define DELETE_ARRAY_3D(array, dim1, dim2) (void)0;

#else /* !USE_VLA */
#define NEW_ARRAY(type, array, size)                         \
	type *array = (type *)malloc((size) * sizeof(type)); \
	if (array == NULL)                                   \
	{                                                    \
		perror("Memory allocation failed");          \
		exit(EXIT_FAILURE);                          \
	}

#define NEW_ARRAY_2D(type, array, rows, cols)                     \
	type **array = (type **)malloc((rows) * sizeof(type *));  \
	if (array == NULL)                                        \
	{                                                         \
		perror("Memory allocation failed");               \
		exit(EXIT_FAILURE);                               \
	}                                                         \
	for (int i = 0; i < (rows); i++)                          \
	{                                                         \
		array[i] = (type *)malloc((cols) * sizeof(type)); \
		if (array[i] == NULL)                             \
		{                                                 \
			perror("Memory allocation failed");       \
			exit(EXIT_FAILURE);                       \
		}                                                 \
	}

#define NEW_ARRAY_3D(type, array, dim1, dim2, dim3)                        \
	type ***array = (type ***)malloc(dim1 * sizeof(type **));          \
	if (array == NULL)                                                 \
	{                                                                  \
		perror("Memory allocation failed");                        \
		exit(EXIT_FAILURE);                                        \
	}                                                                  \
	for (int i = 0; i < dim1; i++)                                     \
	{                                                                  \
		array[i] = (type **)malloc(dim2 * sizeof(type *));         \
		if (array[i] == NULL)                                      \
		{                                                          \
			perror("Memory allocation failed");                \
			exit(EXIT_FAILURE);                                \
		}                                                          \
		for (int j = 0; j < dim2; j++)                             \
		{                                                          \
			array[i][j] = (type *)malloc(dim3 * sizeof(type)); \
			if (array[i][j] == NULL)                           \
			{                                                  \
				perror("Memory allocation failed");        \
				exit(EXIT_FAILURE);                        \
			}                                                  \
		}                                                          \
	}

#define DELETE_ARRAY(array) \
	free(array);

#define DELETE_ARRAY_2D(array, rows)     \
	for (int i = 0; i < (rows); i++) \
	{                                \
		free(array[i]);          \
	}                                \
	free(array);

#define DELETE_ARRAY_3D(array, dim1, dim2)     \
	for (int i = 0; i < dim1; i++)         \
	{                                      \
		for (int j = 0; j < dim2; j++) \
		{                              \
			free(array[i][j]);     \
		}                              \
		free(array[i]);                \
	}                                      \
	free(array);

#endif /* !USE_VLA */

#endif /* sss_PLATFORM_H */

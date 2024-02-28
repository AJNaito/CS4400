/*******************************************
 * Solutions for the CS:APP Performance Lab
 ********************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following student struct 
 */
student_t student = {
  "Aidan Naito",     /* Full name */
  "u1280587@utah.edu",  /* Email address */
};

/***************
 * COMPLEX KERNEL
 ***************/

/******************************************************
 * Your different versions of the complex kernel go here
 ******************************************************/

/* 
 * naive_complex - The naive baseline version of complex 
 */
char naive_complex_descr[] = "naive_complex: Naive baseline implementation";
void naive_complex(int dim, pixel *src, pixel *dest)
{
  int i, j;

  for(i = 0; i < dim; i++)
    for(j = 0; j < dim; j++)
    {

      dest[RIDX(dim - j - 1, dim - i - 1, dim)].red = ((int)src[RIDX(i, j, dim)].red +
						      (int)src[RIDX(i, j, dim)].green +
						      (int)src[RIDX(i, j, dim)].blue) / 3;
      
      dest[RIDX(dim - j - 1, dim - i - 1, dim)].green = ((int)src[RIDX(i, j, dim)].red +
							(int)src[RIDX(i, j, dim)].green +
							(int)src[RIDX(i, j, dim)].blue) / 3;
      
      dest[RIDX(dim - j - 1, dim - i - 1, dim)].blue = ((int)src[RIDX(i, j, dim)].red +
						       (int)src[RIDX(i, j, dim)].green +
						       (int)src[RIDX(i, j, dim)].blue) / 3;

    }
}
/*
char V1Descr[] = "Version 1: unrolling + strength reduction";
void complexImplementationV1(int dim, pixel *src, pixel *dest) {
  int i, j, curSrcPos, curDestPos;

  float divisor = 1.0/3.0;

  unsigned short avg1, avg2, avg3, avg4;

  for (i = 0; i < dim; i++) {
    curSrcPos = i * dim;
    curDestPos = (dim - 1) * dim + (dim - i -1);

    for (j = 0; j < dim; j += 4) {
      avg1 = (unsigned short)((src[curSrcPos].red + src[curSrcPos].blue + src[curSrcPos].green) * divisor);
      avg2 = (unsigned short)((src[curSrcPos + 1].red + src[curSrcPos + 1].blue + src[curSrcPos + 1].green) * divisor);
      avg3 = (unsigned short)((src[curSrcPos + 2].red + src[curSrcPos + 2].blue + src[curSrcPos + 2].green) * divisor);
      avg4 = (unsigned short)((src[curSrcPos + 3].red + src[curSrcPos + 3].blue + src[curSrcPos + 3].green) * divisor);

      dest[curDestPos].red = avg1;
      dest[curDestPos].green = avg1;
      dest[curDestPos].blue = avg1;
      
      dest[curDestPos - dim].red = avg2;
      dest[curDestPos - dim].blue = avg2;
      dest[curDestPos - dim].green = avg2;

      dest[curDestPos - dim - dim].red = avg3;
      dest[curDestPos - dim - dim].green = avg3;
      dest[curDestPos - dim - dim].blue = avg3;
      
      dest[curDestPos - dim - dim - dim].red = avg4;
      dest[curDestPos - dim - dim - dim].green = avg4;
      dest[curDestPos - dim - dim - dim].blue = avg4;

      curDestPos -= 4 * dim;
      curSrcPos += 4;
    }
  }
}

char V2Descr[] = "Loop Unrolling 4x4 (unrolll with local var) + Strength Reduction + Code Motion";
void complexImplementationV2(int dim, pixel* src, pixel* dst) {
  int i, j, curSrc, curDst;

  unsigned int avg1, avg2, avg3, avg4;

  pixel pix1, pix2, pix3, pix4;

  float divisor = 1.0/3.0;
  for (i = 0; i < dim; i ++) {
    curSrc = i * dim;
    curDst = (dim - 1) * dim + (dim - 1 - i);

    for (j = 0; j < dim; j += 4) {
      avg1 = ((int)src[curSrc].red + (int)src[curSrc].green + (int)src[curSrc].blue) * divisor;
      avg2 = ((int)src[curSrc + 1].red + (int)src[curSrc + 1].green + (int)src[curSrc + 1].blue) * divisor;
      avg3 = ((int)src[curSrc + 2].red + (int)src[curSrc + 2].green + (int)src[curSrc + 2].blue) * divisor;
      avg4 = ((int)src[curSrc + 3].red + (int)src[curSrc + 3].green + (int)src[curSrc + 3].blue) * divisor;

      pix1.red = avg1;
      pix1.green = avg1;
      pix1.blue = avg1;

      pix2.red = avg2;
      pix2.blue = avg2;
      pix2.green = avg2;

      pix3.red = avg3;
      pix3.green = avg3;
      pix3.blue = avg3;
      
      pix4.red = avg4;
      pix4.green = avg4;
      pix4.blue = avg4;

      dst[curDst] = pix1;
      dst[curDst - dim] = pix2;
      dst[curDst - dim - dim] = pix3;
      dst[curDst - dim - dim - dim] = pix4;

      curSrc += 4;
      curDst -= 4 * dim;
    }
  }
}
*/

char V3Descr[] = "Loop Unrolling 4x4 (Copy Src data into local var) + Strength Reduction + Code Motion";
void complexImplementationV3(int dim, pixel* src, pixel* dst) {
  int i, j, curSrc, curDst;

  float divisor = 1.0/3.0;
  for (i = 0; i < dim; i ++) {
    curSrc = i * dim;
    curDst = (dim - 1) * dim + (dim - 1 - i);

    pixel pix1, pix2, pix3, pix4;
    for (j = 0; j < dim; j += 4) {
      pix1 = src[curSrc];
      pix2 = src[curSrc + 1];
      pix3 = src[curSrc + 2];
      pix4 = src[curSrc + 3];

      pix1.red = (((int)pix1.red + (int)pix1.green + (int)pix1.blue) * divisor);
      pix1.green = pix1.red;
      pix1.blue = pix1.red;

      pix2.red = (((int)pix2.red + (int)pix2.green + (int)pix2.blue) * divisor);
      pix2.blue = pix2.red;
      pix2.green = pix2.red;

      pix3.red = (((int)pix3.red + (int)pix3.green + (int)pix3.blue) * divisor);
      pix3.green = pix3.red;
      pix3.blue = pix3.red;
      
      pix4.red = (((int)pix4.red + (int)pix4.green + (int)pix4.blue) * divisor);
      pix4.green = pix4.red;
      pix4.blue = pix4.red;

      dst[curDst] = pix1;
      dst[curDst - dim] = pix2;
      dst[curDst - dim - dim] = pix3;
      dst[curDst - dim - dim - dim] = pix4;

      curSrc += 4;
      curDst -= 4 * dim;
    }
  }
}

char complexV4_descr[] = "Blocking Technique (W = 8) and saving to local variable";
void complexImplementationV4(int dim, pixel* src, pixel* dst) {
  int i, j, ii, jj, W = 8, curSrcPos, curDstPos;

  pixel curSrc;
  float divisor = 1.0/3.0;
  for (i = 0; i < dim; i += W) {
    for (j = 0; j < dim; j += W) {
      for (ii = i; ii < i + W; ii++) {
        curSrcPos = ii * dim + j;
        curDstPos = (dim - j- 1) * dim + (dim - 1 - ii);

        for (jj = j; jj < j + W; jj++) {
          curSrc = src[curSrcPos];

          curSrc.red = (((int)curSrc.red + (int)curSrc.green + (int)curSrc.blue) * divisor);
          curSrc.green = curSrc.red;
          curSrc.blue = curSrc.red;

          dst[curDstPos] = curSrc;

          curSrcPos ++;
          curDstPos -= dim;
        }
      }
    }
  }
}


/* 
 * complex - Your current working version of complex
 * IMPORTANT: This is the version you will be graded on
 */
char complex_descr[] = "complex: Current working version (Version 4)";
void complex(int dim, pixel *src, pixel *dest)
{
  complexImplementationV4(dim, src, dest);
}

/*********************************************************************
 * register_complex_functions - Register all of your different versions
 *     of the complex kernel with the driver by calling the
 *     add_complex_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_complex_functions() {
  add_complex_function(&complex, complex_descr);
  add_complex_function(&naive_complex, naive_complex_descr);
  //add_complex_function(&complexImplementationV1, V1Descr);
  //add_complex_function(&complexImplementationV2, V2Descr);
  add_complex_function(&complexImplementationV3, V3Descr);
  add_complex_function(&complexImplementationV4, complexV4_descr);
}


/***************
 * MOTION KERNEL
 **************/

/***************************************************************
 * Various helper functions for the motion kernel
 * You may modify these or add new ones any way you like.
 **************************************************************/

typedef struct {
  int red;
  int blue;
  int green;
} pixel_sum;
/* 
 * weighted_combo - Returns new pixel value at (i,j) 
 */
static pixel weighted_combo(int dim, int i, int j, pixel *src) 
{
  int ii, jj;
  pixel current_pixel;

  int red, green, blue;
  red = green = blue = 0;

  int num_neighbors = 0;
  for(ii=0; ii < 3; ii++)
    for(jj=0; jj < 3; jj++) 
      if ((i + ii < dim) && (j + jj < dim)) 
      {
	      num_neighbors++;
	      red += (int) src[RIDX(i+ii,j+jj,dim)].red;
	      green += (int) src[RIDX(i+ii,j+jj,dim)].green;
	      blue += (int) src[RIDX(i+ii,j+jj,dim)].blue;
      }
  
  current_pixel.red = (unsigned short) (red / num_neighbors);
  current_pixel.green = (unsigned short) (green / num_neighbors);
  current_pixel.blue = (unsigned short) (blue / num_neighbors);
  
  return current_pixel;
}

// static __attribute__((always_inline)) inline pixel weighted_combo_V2(int dim, int i, int j, pixel* src) {
//   int ii, jj, iBound = 3, jBound = 3;

//   double divisor;

//   pixel current_pixel;

//   // we know sum of all values of chars will be > 0 
//   int red, green, blue, position;
//   red = green = blue = position = 0;

//   if (i + 3 > dim)
//     iBound = dim - i;

//   if (j + 3 > dim)
//     jBound = dim - j;

//   int num_neighbors = 0;
//   position = (i * dim) + j;
//   for (ii = 0; ii < iBound; ii++) {
//     for (jj = 0; jj < jBound; jj++) {
//       num_neighbors++;

//       red += src[position + jj].red;
//       green += src[position + jj].green;
//       blue += src[position + jj].blue;
//     }

//     position += dim;
//   }

//   divisor = 1.0/(double)num_neighbors;

//   current_pixel.red = (unsigned short)(red * divisor);
//   current_pixel.green = (unsigned short)(green * divisor);
//   current_pixel.blue = (unsigned short)(blue * divisor);

//   return current_pixel;
// }

static __attribute__((always_inline)) inline pixel weighted_combo_3x3(int dim, int position, pixel_sum* sum1, pixel_sum* sum2, pixel* src, double divisor) {
  int ii;

  // Sum up shared two columns
  int red, green, blue;
  red = sum1->red + sum2->red;
  blue = sum1->blue + sum2->blue;
  green = sum1->green + sum2->green;

  // replace sum1's values with sum2
  sum1->red = sum2->red;
  sum1->blue = sum2->blue;
  sum1->green = sum2->green;

  // zero out sum2 values to get values of next column
  sum2->red = 0;
  sum2->green = 0;
  sum2->blue = 0;
  
  //Get values of last column
  position += 2;
  for (ii = 0; ii < 3; ii++) {
    sum2->red += src[position].red;
    sum2->green += src[position].green;
    sum2->blue += src[position].blue;

    position += dim;
  }
  //sum1 and sum2 are shared with the next pixel
  red += sum2->red;
  blue += sum2->blue;
  green += sum2->green;

  pixel current_pixel;
  current_pixel.red = (unsigned short)(red * divisor);
  current_pixel.blue = (unsigned short)(blue * divisor);
  current_pixel.green = (unsigned short)(green * divisor);

  return current_pixel;
}

static __attribute__((always_inline)) inline pixel weighted_combo_3x3_orig(int dim, int position, pixel* src, double divisor) {
  int ii, jj;

  // Sum up shared two columns
  int red, green, blue;
  red = blue = green = 0;
  
  //Get values of last column
  pixel current_pixel;
  for (ii = 0; ii < 3; ii++) {
    for (jj = 0; jj < 3; jj++) {
      red += src[position + jj].red;
      green += src[position + jj].green;
      blue += src[position + jj].blue;
    }

    position += dim;
  }
  current_pixel.red = (unsigned short)(red * divisor);
  current_pixel.blue = (unsigned short)(blue * divisor);
  current_pixel.green = (unsigned short)(green * divisor);

  return current_pixel;
}

static __attribute__((always_inline)) inline pixel weighted_combo_3x2(int dim, int j, int position, pixel_sum* sum1, pixel* src, double divisor) {
  int ii, red, green, blue;
  red = sum1->red;
  blue = sum1->blue;
  green = sum1->green;

  pixel current_pixel;

// if j is second to last in column
  if (j == dim - 2) {
    //Only want last column
    position += 1;

    sum1->red = 0;
    sum1->green = 0;
    sum1->blue = 0;

    for (ii = 0; ii < 3; ii++) {
      //replace sum1's values with last column's pixel values
      sum1->red += src[position].red;
      sum1->green += src[position].green;
      sum1->blue += src[position].blue;

      position += dim;
    }

    // add new sum
    red += sum1->red;
    green += sum1->green;
    blue += sum1->blue;
  } else {
    divisor = 1.0/3.0;
  }

  current_pixel.red = (unsigned short)(red * divisor);
  current_pixel.green = (unsigned short)(green * divisor);
  current_pixel.blue = (unsigned short)(blue * divisor);

  return current_pixel;
}

static __attribute__((always_inline)) inline pixel weighted_combo_3x2_orig(int dim, int j, int position, pixel* src, double divisor) {
  int ii, jj, jBound = 2, red, green, blue;
  red = green = blue = 0;

  pixel current_pixel;

  if (j > dim - 2) {
    divisor = 1.0/3.0;
    jBound = 1;
  }

  for (ii = 0; ii < 3; ii++) {
    for (jj = 0; jj < jBound; jj++) {
      red += (int)src[position + jj].red;
      green += (int)src[position + jj].green;
      blue += (int)src[position + jj].blue;
    }

    position += dim;
  }

  current_pixel.red = (unsigned short)(red * divisor);
  current_pixel.green = (unsigned short)(green * divisor);
  current_pixel.blue = (unsigned short)(blue * divisor);

  return current_pixel;
}

static __attribute__((always_inline)) inline pixel weighted_combo_2x3(int dim, int i, int position, pixel_sum* sum1, pixel_sum* sum2, pixel* src, double divisor) {
  int ii, iBound = 2, red, green, blue;
  //initialize red, blue, green with sums of last two columns pixels
  red = sum1->red + sum2->red;
  blue = sum1->blue + sum2->blue;
  green = sum1->green + sum2->green;

  pixel current_pixel;

  if (i > dim - 2) {
    divisor = 1.0/3.0;
    iBound = 1;
  }
  
  sum1->red = sum2->red;
  sum1->blue = sum2->blue;
  sum1->green = sum2->green;

  sum2->red = 0;
  sum2->green = 0;
  sum2->blue = 0;

  // only want last column
  position += 2;
  for (ii = 0; ii < iBound; ii++) {
    //replace sum2 with sum of last column pixels
    sum2->red += src[position].red;
    sum2->blue += src[position].blue;
    sum2->green += src[position].green;

    position += dim;
  }

  // add last column pixels
  red += sum2->red;
  green += sum2->green;
  blue += sum2->blue;

  current_pixel.red = (unsigned short)(red * divisor);
  current_pixel.green = (unsigned short)(green * divisor);
  current_pixel.blue = (unsigned short)(blue * divisor);
  
  return current_pixel;
}

static __attribute__((always_inline)) inline pixel weighted_combo_2x3_original(int dim, int i, int position, pixel* src, double divisor) {
  int ii, jj, iBound = 2, red, green, blue;
  red = blue = green = 0;

  pixel current_pixel;

  if (i > dim - 2) {
    divisor = 1.0/3.0;
    iBound = 1;
  }

  for (ii = 0; ii < iBound; ii++) {
    for (jj = 0; jj < 3; jj++) {
      red += (int)src[position + jj].red;
      blue += (int)src[position + jj].blue;
      green += (int)src[position + jj].green;
    }

    position += dim;
  }
  current_pixel.red = (unsigned short)(red * divisor);
  current_pixel.green = (unsigned short)(green * divisor);
  current_pixel.blue = (unsigned short)(blue * divisor);
  
  return current_pixel;
}

static __attribute__((always_inline)) inline pixel weighted_combo_2x2(int dim, int i, int j, int position, pixel* src, double divisor) {
  int red, green, blue;
  red = green = blue = 0;

  pixel current_pixel;

  if (i == dim - 2 && j == dim - 2) {
    red += src[position].red + src[position + 1].red + src[position + dim].red + src[position + dim + 1].red;
    green += src[position].green + src[position + 1].green + src[position + dim].green + src[position + dim + 1].green;
    blue += src[position].blue + src[position + 1].blue + src[position + dim].blue + src[position + dim + 1].blue;
  } else if (i == dim - 1 && j != dim - 1) {
    red += src[position].red + src[position + 1].red;
    green += src[position].green + src[position + 1].green;
    blue += src[position].blue + src[position + 1].blue;
  } else {
    red += src[position].red + src[position + dim].red;
    green += src[position].green + src[position + dim].green;
    blue += src[position].blue + src[position + dim].blue;
  }

  current_pixel.red = (unsigned short)(red * divisor);
  current_pixel.blue = (unsigned short)(blue * divisor);
  current_pixel.green = (unsigned short)(green * divisor);

  return current_pixel;
}

static __attribute__((always_inline)) inline void weighted_combo_V3(int dim, int i, int j, int position, pixel* src, pixel* dst) {
  int ii, jj, iBound = 3, jBound = 3;

  double divisor;

  // we know sum of all values of chars will be > 0 
  int red, green, blue;
  red = green = blue = 0;
  if (i + 3 > dim)
    iBound = dim - i;

  if (j + 3 > dim)
    jBound = dim - j;

  int num_neighbors = 0;
  for (ii = 0; ii < iBound; ii++) {
    for (jj = 0; jj < jBound; jj++) {
      num_neighbors++;

      red += (int)src[position + jj].red;
      green += (int)src[position + jj].green;
      blue += (int)src[position + jj].blue;
    }

    position += dim;
  }

  divisor = 1.0/(double)num_neighbors;

  dst->red = (unsigned short)(red * divisor);
  dst->green = (unsigned short)(green * divisor);
  dst->blue = (unsigned short)(blue * divisor);
}

/// @brief Calculates the initial column value
/// @param col1 - 1st column
/// @param col2 - second column
/// @param i - row position
/// @param j - column position
/// @param dim - Size of matrix (dim x dim)
/// @param src - array of pixels
static __attribute__((always_inline)) inline void calcInitCols(pixel_sum* col1, pixel_sum* col2, int i, int j, int dim, pixel* src) {
  int ii, iBound = 3, pos =  i * dim + j;

  if (i >= dim - 2)
    iBound = dim - i;

  if (j + 1 == dim - 1) {
    for (ii = 0; ii < 3; ii++) {
      col1->red += src[pos].red;
      col1->blue += src[pos].blue;
      col1->green += src[pos].green;

      pos += dim;
    }
  } else {
    for (ii = 0; ii < iBound; ii++) {
      col1->red += src[pos].red;
      col1->green += src[pos].green;
      col1->blue += src[pos].blue;

      col2->red += src[pos + 1].red;
      col2->green += src[pos + 1].green;
      col2->blue += src[pos + 1].blue;

      pos += dim;
    }
  }
}



/******************************************************
 * Your different versions of the motion kernel go here
 ******************************************************/


/*
 * naive_motion - The naive baseline version of motion 
 */
char naive_motion_descr[] = "naive_motion: Naive baseline implementation";
void naive_motion(int dim, pixel *src, pixel *dst) 
{
  int i, j;
    
  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      dst[RIDX(i, j, dim)] = weighted_combo(dim, i, j, src);
}

/*
char motV1Desc[] = "Motion V1: Unrolling + V1 weighted Combo";
void motionV1(int dim, pixel *src, pixel* dst) {
  int i, j, curPos = 0;

  for (i = 0; i < dim; i++) {
    curPos = i * dim;

    for (j = 0; j < dim; j += 4) {
      dst[curPos] = weighted_combo_V1(dim, i, j, src);
      dst[curPos + 1] = weighted_combo_V1(dim, i, j, src);
      dst[curPos + 2] = weighted_combo_V1(dim, i, j, src);
      dst[curPos + 3] = weighted_combo_V1(dim, i, j, src);

      curPos += 4;
    }
  }
}
*/
/*
char motV2Desc[] = "Motion V2: Unrolling + V2 weighted combo";
void motionV2(int dim, pixel* src, pixel* dst) {
  int i, j, curPos = 0;

  for (i = 0; i < dim; i++) {
    curPos = i * dim;
    for (j = 0; j < dim; j +=8) {
      dst[curPos] = weighted_combo_V2(dim, i, j, src);
      dst[curPos + 1] = weighted_combo_V2(dim, i, j + 1, src);
      dst[curPos + 2] = weighted_combo_V2(dim, i, j + 2, src);
      dst[curPos + 3] = weighted_combo_V2(dim, i, j + 3, src);
      dst[curPos + 4] = weighted_combo_V2(dim, i, j + 4, src);
      dst[curPos + 5] = weighted_combo_V2(dim, i, j + 5, src);
      dst[curPos + 6] = weighted_combo_V2(dim, i, j + 6, src);
      dst[curPos + 7] = weighted_combo_V2(dim, i, j + 7, src);

      curPos += 8;
    }
  }
}

char motV3Desc[] = "Motion V3: Blocking Technique (W = 32) + unrolling + V2 weighted combo";
void motionV3(int dim, pixel* src, pixel* dst) {
  int i, j, curPos = 0;

  for (i = 0; i < dim; i++) {
    curPos = i * dim;
    for (j = 0; j < dim; j +=8) {
      dst[curPos] = weighted_combo_V2(dim, i, j, src);
      dst[curPos + 1] = weighted_combo_V2(dim, i, j + 1, src);
      dst[curPos + 2] = weighted_combo_V2(dim, i, j + 2, src);
      dst[curPos + 3] = weighted_combo_V2(dim, i, j + 3, src);
      dst[curPos + 4] = weighted_combo_V2(dim, i, j + 4, src);
      dst[curPos + 5] = weighted_combo_V2(dim, i, j + 5, src);
      dst[curPos + 6] = weighted_combo_V2(dim, i, j + 6, src);
      dst[curPos + 7] = weighted_combo_V2(dim, i, j + 7, src);

      curPos += 8;
    }
  }
}
*/

/*
// split one for loop into 3 separate, calculate bottom right 2x2 explicitly
char motV4Desc[] = "Motion V4: Split calculations";
void motionV4(int dim, pixel* src, pixel* dst) {
  int i, j, curPos = 0;
  
  float divisor = 1.0/9.0;
  for (i = 0; i < dim - 2; i++) {
    for (j = 0; j < dim - 2; j++) {
      dst[curPos + j] = weighted_combo_3x3_orig(dim, curPos + j, src, divisor);
    }

    curPos += dim;
  }

  divisor = 1.0/6.0;
  curPos = 0;
  for (i = 0; i < dim - 2; i++) {
    for (j = dim - 2; j < dim; j+=2) {
      dst[curPos + j] = weighted_combo_3x2_orig(dim, j, curPos + j, src, divisor);
      dst[curPos + j + 1] = weighted_combo_3x2_orig(dim, j + 1, curPos + j + 1, src, divisor);
    }
    curPos += dim;
  }

  curPos = (dim - 2) * dim;
  for (i = dim - 2; i < dim; i++) {
    for (j = 0; j < dim - 2; j+=8) {
      dst[curPos + j] = weighted_combo_2x3_original(dim, i, curPos + j, src, divisor);
      dst[curPos + j + 1] = weighted_combo_2x3_original(dim, i, curPos + j + 1, src, divisor);
      dst[curPos + j + 2] = weighted_combo_2x3_original(dim, i, curPos + j + 2, src, divisor);
      dst[curPos + j + 3] = weighted_combo_2x3_original(dim, i, curPos + j + 3, src, divisor);
      dst[curPos + j + 4] = weighted_combo_2x3_original(dim, i, curPos + j + 4, src, divisor);
      dst[curPos + j + 5] = weighted_combo_2x3_original(dim, i, curPos + j + 5, src, divisor);
      dst[curPos + j + 6] = weighted_combo_2x3_original(dim, i, curPos + j + 6, src, divisor);
      dst[curPos + j + 7] = weighted_combo_2x3_original(dim, i, curPos + j + 7, src, divisor);
    }

    curPos += dim;
  }

  i = dim - 2;
  j = dim - 2;
  curPos = dim * i + j;
  dst[curPos] = weighted_combo_2x2(dim, i, j, curPos, src, 1.0/4.0);
  dst[curPos + 1] = weighted_combo_2x2(dim, i, j + 1, curPos + 1, src, 1.0/2.0);
  dst[curPos + dim] = weighted_combo_2x2(dim, i + 1, j, curPos + dim, src, 1.0/2.0);


  // Last pixel is just itself
  curPos = dim * (dim - 1) + (dim - 1);
  dst[curPos] = src[curPos];
} */

char motV5Desc[] = "Motion V5: Split calculations + recycling sums";
void motionV5(int dim, pixel* src, pixel* dst) {
  int i, j, curPos = 0;
  
  float divisor = 1.0/9.0;

  pixel_sum col1;
  pixel_sum col2;

  for (i = 0; i < dim - 2; i++) {
    col1.red = col1.green = col1.blue = 0;
    col2.red = col2.blue = col2.green  = 0;

    calcInitCols(&col1, &col2, i, 0, dim, src);

    for (j = 0; j < dim - 2; j++) {
      dst[curPos + j] = weighted_combo_3x3(dim, curPos + j, &col1, &col2, src, divisor);
    }

    curPos += dim;
  }

  divisor = 1.0/6.0;
  curPos = 0;
  for (i = 0; i < dim - 2; i++) {
    col1.red = col1.green = col1.blue = 0;
    col2.red = col2.green = col2.blue = 0;

    calcInitCols(&col1, &col2, i, dim - 2, dim, src);
    for (j = dim - 2; j < dim; j++) {
      dst[curPos + j] = weighted_combo_3x2(dim, j, curPos + j, &col1, src, divisor);
    }

    curPos += dim;
  }

  curPos = (dim - 2) * dim;
  for (i = dim - 2; i < dim; i++) {
    col1.red = col1.green = col1.blue = 0;
    col2.red = col2.green = col2.blue = 0;
    calcInitCols(&col1, &col2, i, 0, dim, src);

    for (j = 0; j < dim - 2; j++) {
      dst[curPos + j] = weighted_combo_2x3(dim, i, curPos + j, &col1, &col2, src, divisor);
    }

    curPos += dim;
  }

  i = dim - 2;
  j = dim - 2;
  curPos = dim * i + j;
  dst[curPos] = weighted_combo_2x2(dim, i, j, curPos, src, 1.0/4.0);
  dst[curPos + 1] = weighted_combo_2x2(dim, i, j + 1, curPos + 1, src, 1.0/2.0);
  dst[curPos + dim] = weighted_combo_2x2(dim, i + 1, j, curPos + dim, src, 1.0/2.0);


  // Last pixel is just itself
  curPos = dim * (dim - 1) + (dim - 1);
  dst[curPos] = src[curPos];
}

/*
 * motion - Your current working version of motion. 
 * IMPORTANT: This is the version you will be graded on
 */
char motion_descr[] = "motion: Working Version (V5)";
void motion(int dim, pixel *src, pixel *dst) 
{
  motionV5(dim, src, dst);
}

/********************************************************************* 
 * register_motion_functions - Register all of your different versions
 *     of the motion kernel with the driver by calling the
 *     add_motion_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_motion_functions() {
  add_motion_function(&motion, motion_descr);
  // add_motion_function(&motionV1, motV1Desc);
  add_motion_function(&naive_motion, naive_motion_descr);
  //add_motion_function(&motionV2, motV2Desc);
  //add_motion_function(&motionV3, motV3Desc);
  //add_motion_function(&motionV4, motV4Desc);
  add_motion_function(&motionV5, motV5Desc);
}

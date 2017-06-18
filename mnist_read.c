/*Visual Studio와의 호환성 확보 */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mnist-utils.h"


int main()
{
	int end=1;

	FILE *imageFile, *labelFile;
	imageFile = openMNISTImageFile(MNIST_TRAINING_SET_IMAGE_FILE_NAME);
	labelFile = openMNISTLabelFile(MNIST_TRAINING_SET_LABEL_FILE_NAME);




	while (end)
	{
		// Reading next image and corresponding label
		MNIST_Image img = getImage(imageFile);
		MNIST_Label lbl = getLabel(labelFile);

		displayImage(&img, 6, 6);

		printf("type 0 to end program\n");
		scanf("%d", &end);
	}
}


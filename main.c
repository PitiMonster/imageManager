#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFF_SIZE 1024

typedef struct image{
	int height;
	int width;
	int grayscale;
	char name[2];
	int** matrix;
} image;

struct Node{
	image *img;
	// to jest nazwa dla konkretnego obrazka ktorego chcemy zapisać do tablicy
	char *imgName;

	struct Node *next;
};

void clear_memory(int ** matrix, int height);
void show_image(image *img);
void rotate90(image *img);
void rotate(image *img, int k);
void negative(image *img);
void salt_and_pepper(image *img, int density);
void histogram(image *img, int * result);
void histogram_equalize(image *img);
void histogram_paint(image *img);
void histogram_save(image *img, char * path);
void filter(image *img, int filter_size, int ** weights);
void gauss_filter(image *img);
void writeFile(image *img);
void addImage(struct Node** head_ref, image *newImage, size_t dataSize);
void deleteImage(struct Node **headRef);
void printList(struct Node* node);
image *selectCurrImage(struct Node **headRef);

// tak ogólnie to tu dodajesz te obrazki na przód zawsze i ogólnie jak chcesz edytować ten pierwszy obrazek z talicy to się coś psuje i go później krzywo wypisuje więc bierz te wcześniejsze do edycji na testach xDDDDD
void addImage(struct Node** head_ref, image *newImage, size_t dataSize){

	char *imageName =  (char*)malloc(BUFF_SIZE*(sizeof(char)));
	printf("Provide name for this image: ");
	scanf("%s", imageName);

	//Allocate memory for node
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));

	newNode->img = malloc(dataSize);
	newNode->next = (*head_ref);
	newNode->imgName = imageName;
	newNode->img->height = newImage->height;
	newNode->img->width = newImage->width;
	newNode->img->grayscale = newImage->grayscale;
	newNode->img->name[0] = newImage->name[0];
	newNode->img->name[1] = newImage->name[1];

	newNode->img->matrix = malloc(newNode->img->height * sizeof(int*));
	for (int i = 0; i < newNode->img->height; i++)
		newNode->img->matrix[i] = malloc(newNode->img->width * sizeof(int));

	for(int i = 0; i < newImage->height; i++){
		for(int j = 0; j < newImage->width; j++){
			newNode->img->matrix[i][j] = newImage->matrix[i][j];
		}
	}
	(*head_ref) = newNode;
}

// delete image from array by key = name of image
void deleteImage(struct Node **headRef){

	char *imageName =  (char*)malloc(BUFF_SIZE*(sizeof(char)));
	printf("Provide name of image to delete: ");
	scanf("%s", imageName);
	
	struct Node* temp = *headRef, *prev;

	// if head node itself holds the key to be deleted
	if(temp != NULL && strcmp(temp->imgName,imageName) == 0){
		*headRef = temp->next; // change head for the next element
		clear_memory(temp->img->matrix,temp->img->height);
		free(temp); // free old head
		return;
	}

	//search for image which cointains name = key
	while( temp != NULL && strcmp(temp->imgName,imageName) != 0){
		prev = temp;
		temp = temp->next;
	}

	// If key was not present in linked list
	if(temp == NULL){
		printf("Image with that name could not be found!");
		return;
	}
	printf("Image deleted!");
	// Unlink the node from linked list
	prev->next = temp->next;
	clear_memory(temp->img->matrix,temp->img->height);
	free(temp);
	return;

}

void printList(struct Node* node){
	while(node != NULL){
		int i = 0, j = 0;
		printf("%s\n", node->img->name);
		printf("%d %d\n", node->img->height, node->img->width);
		printf("%d\n", node->img->grayscale);
		for (i = 0; i < node->img->height; i++){
			for (j = 0; j < node->img->width; j++){
				printf("%d ", node->img->matrix[i][j]);
			}
		printf("\n");
		}
		printf("\n");

		node = node->next;
	}
}

image *selectCurrImage(struct Node **headRef){

	char *imageName = (char*)malloc(BUFF_SIZE*(sizeof(char)));
	printf("Provide name of  image to select: ");
	scanf("%s", imageName);

	struct Node* temp = *headRef, *prev;

	//search for image which cointains name = key
	while( temp != NULL && strcmp(temp->imgName,imageName) != 0){
		prev = temp;
		temp = temp->next;
	}

	// If key was not present in linked list
	if(temp == NULL){
		printf("Image with that name could not be found!\n");
		return NULL;
	}
	printf("Image selected!\n");

	return temp->img;
	
}

void clear_memory(int ** matrix, int height){
	for (int i = 0; i < height; i++)
		free(matrix[i]);
	free(matrix);
};

void show_image(image *img)
{
	int i = 0, j=0;
	printf("%s\n", img->name);
	for (i = 0; i < img->height; i++){
		for (j = 0; j < img->width; j++){
			printf("%d ", img->matrix[i][j]);
		}
		printf("\n");
	}
}

void rotate90(image *img){
	int ** rotated_image = (int**)malloc(img->width * sizeof(int*));
	if(rotated_image != NULL){
		for (int i = 0; i < img->width; i++){
			rotated_image[i] = (int*)malloc(img->height * sizeof(int));
			if(rotated_image[i]){
				for(int j = 0; j < img->height; j++){
					rotated_image[i][j] = img->matrix[img->height - j - 1][i];
				}
			}
		}
	}

	int temp = img -> width;
	img->width = img->height;
	img->height = temp;

	free(img->matrix);

	img->matrix = rotated_image;
	
}

void rotate(image *img, int k){
	for(int i=0; i < k%4; i++)
		rotate90(img);
}

void negative(image *img){
	for (int i = 0; i < img->height; i++){
		for (int j = 0; j < img->width; j++){
			img->matrix[i][j] = img->grayscale - img->matrix[i][j]; 
		}
	}
}

void salt_and_pepper(image *img, int noise_density){
	for (int i = 0; i < img->height; i++){
		for (int j = 0; j < img->width; j++){
			if(rand() % 100 <= noise_density){
				if(rand() % 2 == 0)
					img->matrix[i][j] = 0;
				else
					img->matrix[i][j] = img->grayscale;
			}
		}
	}
}

void histogram(image *img, int * result){
	int level = img->grayscale;

	for (int i = 0; i < level; i++)
		result[i] = 0;

	for (int i = 0; i < img->height; i++){
		for (int j = 0; j < img->width; j++){
			result[img->matrix[i][j]]++;
		}
	}
}

void histogram_equalize(image *img){
	int level = img->grayscale;
	int sum = 0;

	double cdf[level];
	int LUT[level];
	int * histogram_values = (int*)malloc(level * sizeof(int));
	histogram(img, histogram_values); 

	for(int i = 0; i<level; i++){
		sum += histogram_values[i];
		cdf[i] = sum;
	}

	for(int i = 0; i<level; i++){
		cdf[i] /= sum;
		LUT[i] = (int) ((cdf[i] - cdf[0]) / (1 - cdf[0]) * (level - 1));
	}


	for (int i = 0; i < img->height; i++){
		for (int j = 0; j < img->width; j++){
			img->matrix[i][j] = LUT[img->matrix[i][j]];
		}
	}
}

void histogram_file(image *img, char * path){
	int level = img->grayscale;
	int hist_values[level];

	FILE* histogram_file;
	histogram_file = fopen(path, "w");

	if(histogram == NULL){
		printf("Nie udalo sie otworzyc pliku histogramu");
		return;
	}
	
	histogram(img, hist_values);

	for (int i = 0; i < level; i++){
		fprintf(histogram_file, "%d,%d\n", i, hist_values[i]);
	}
}

void filter(image *img, int filter_size, int ** weights){
	int sumWeights = 0;
	for(int i = 0; i < filter_size; i++){
		for(int j = 0; j < filter_size; j++){
			sumWeights += weights[i][j];
		}
	}

	int ** filtered_image = (int**)malloc(img->height * sizeof(int*));
	if(filtered_image != NULL){
		for (int i = 0; i < img->height; i++){
			filtered_image[i] = (int*)malloc(img->width * sizeof(int));
		}
	}

	int offset = filter_size / 2;
	for (int i = 0; i < img->height; i++){
		for (int j = 0; j < img->height; j++){
			int sum = 0;
			for(int k = 0; k < filter_size; k++){
				for(int l = 0; l < filter_size; l++){
					int neighbour_y = i+k-offset;
					int neighbour_x = j+l-offset;
					if(neighbour_y < 0)
						neighbour_y = 0;
					else if(neighbour_y >= img->height)
						neighbour_y = img->height-1;
					if(neighbour_x < 0)
						neighbour_x = 0;
					else if(neighbour_x >= img->width)
						neighbour_x = img->width-1;
						
					sum += img->matrix[neighbour_y][neighbour_x] * weights[k][l];
				}
			}
			filtered_image[i][j] = sum / sumWeights;
		}
	}

	clear_memory(img->matrix, img->height);

	img->matrix = filtered_image;
}


void gauss_filter(image *img){
	int * weights[3];
	int row0[3] = {1,2,1};
	int row1[3] = {2,4,2};
	int row2[3] = {1,2,1};
	
	weights[0] = row0;
	weights[1] = row1;
	weights[2] = row2;

	filter(img, 3, weights);
}



FILE* readFile() {
	FILE* fp;
	char *filename;
	printf("Podaj sciezke do pliku: ");
	// filename = malloc(sizeof(char) * BUFF_SIZE);
	// scanf("%s", filename);
	filename = "8x8.pgm";
	printf("wczytano:  %s\n", filename);
	fp = fopen(filename, "r");
	// free(filename);
	if (fp == NULL) 
	{
		printf("\nNie ma takiego pliku\n\n");
		return readFile();
	}
	printf("\n jest taki plik\n");
	return fp;
}

image pgmToStruct(FILE *fp)
{
	image wielki;
	int a, b, c;
	char *d = malloc(BUFF_SIZE*sizeof(char));
	fscanf(fp, "%s %d %d %d",d,&wielki.height, &wielki.width, &wielki.grayscale);
	wielki.matrix = malloc(wielki.height * sizeof(int*));
	for (int i = 0; i < wielki.height; i++)
		wielki.matrix[i] = malloc(wielki.width * sizeof(int));
	for (int i = 0; i < wielki.height; i++)
	{
		for (int j = 0; j < wielki.width; j++) {
			fscanf(fp, "%d", &wielki.matrix[i][j]);
		}
	}
	
	wielki.name[0] = d[0];
	wielki.name[1] = d[1];
	free(d);
	return wielki;
}

void writeFile(image *img){
    char name[100];
    char *extension = ".pgm";

    printf("Podaj nazwe pliku:");
    scanf("%s",name);

    char *namePGM = malloc((strlen(name) + strlen(extension) + 1)* sizeof(char));
    namePGM[0] = '\0'; // zapewnia, że pamięć zmiennej jest pusta
    strcat(namePGM,name);
    strcat(namePGM,extension);

    FILE *f = fopen(namePGM,"w");
    if(f == NULL){
        printf("writing error");
    }
    fprintf(f,"%s\n",img->name);
    fprintf(f,"%d %d\n",img->height,img->width);
    fprintf(f,"%d\n",img->grayscale);
    for(int i = 0; i < img->height; i++){
        for(int j = 0; j< img->width; j++){
            fprintf(f,"%d ",img->matrix[i][j]);
        }
        fprintf(f,"\n");
    }
    printf("Saved successfully!");
    free(extension);
    free(namePGM);
    free(f);

}



int main()
{
	struct Node *start = NULL;
	FILE *read = readFile();
	image currImage = pgmToStruct(read);
	printf("wysokosc: %d\n szerokosc: %d\nskala szarosci: %d\n P?: %c%c\n", currImage.height, currImage.width, currImage.grayscale, currImage.name[0],currImage.name[1]);
	//show_image(&currImage);
	//histogram_equalize(&currImage);
	//show_image(&currImage);
	printList(start);
	addImage(&start, &currImage, sizeof(image));
	rotate90(&currImage);
	addImage(&start, &currImage, sizeof(image));
	printList(start);
	image *tempImg = selectCurrImage(&start);
	if(tempImg != NULL){
		currImage = *tempImg;
	}
	rotate90(&currImage);
	addImage(&start, &currImage, sizeof(image));	
	deleteImage(&start);
	printList(start);
	return 0;
}

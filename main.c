#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Nokta {
	int x,y;
};

typedef struct Nokta Nokta;

/*******************************************************************************
 A function that computes the distance between two points p1 and p2 of type Nokta 
   and returns the result as float
*******************************************************************************/
float mesafe(Nokta p1, Nokta p2); 

/*******************************************************************************
 A function that takes as an input txt file  (filename.txt) and reads from it coordinates of points 
 of type struct Nokta and returns the coordinates in an array (arr) of struct Nokta with the total 
 number of points in the file in the variable size
*******************************************************************************/
void readFile(char * filename, Nokta* arr, int* size );

/*******************************************************************************
 A function that takes as input an array of type struct Nokta and the size of the array (n)
 and prints the coordinates of points

*******************************************************************************/
void printStruct(Nokta*  arr, int n);

/*******************************************************************************
 A recursive function that takes as input an array of struct Point, the start index (l), the ending index (r)
 and the type of sort (x): if set to 1 the sorting will be according to x cordinates of the points, else it will
 be according to the y coordinates.
*******************************************************************************/
void mergeSort(Nokta arr[], int start, int finish, int x);

/*******************************************************************************
 A  function that takes as  input an array of struct Point, the start index (l), the ending index (r)
 and the type of sort (x): if set to 1 the sorting will be according to x cordinates of the points, else it will
 be according to the y coordinates.
*******************************************************************************/
void merge(Nokta arr[], int l, int m, int r,int x);

/*******************************************************************************
 A recursive function that takes as input an array of struct Point, the start index (start), the ending index
 (finish) and returns the smallest distance between Points with the coordinates of these points in (a) and (b).
*******************************************************************************/
float divide(Nokta arr[],int start, int finish, Nokta* a, Nokta* b);


int main(int argc, char *argv[]) {
	printf("****This program finds the smallest distance between Points in a plane**** \n");
	int size;
	Nokta a,b; /*Will be used to find the coordinates of the closest points*/
	float d; /*the smalles distance between 2 points*/
	/* Reading the points from the file*/	
	Nokta* arr = malloc( 20 * sizeof(Nokta)); /*Assuming there will be maximum 20 points*/
	readFile("sample.txt", arr, &size);
	/* Sorting points according to their x coordinates*/	
	mergeSort(arr,0, size-1, 1);
	printf("The set of points in the plane: \n");
	printStruct(arr, size);
	d= divide(arr,0,size-1, &a,&b);
	printf("\n*******************************************************************************");
	printf("\nThe final result of the shortest distance  is: %f " , d);
	printf("\nIt is between (%d,%d) and (%d,%d)", a.x,a.y,b.x,b.y);
	return 0;
}

float divide(Nokta arr[],int start, int finish, Nokta* a, Nokta* b){
	float dmin, dl,dr,m, d1,d2,d3; 
	/*dmin is for the minimum distance found so far, dl: the minimum distance iin the left part, 
	dr: the minimum distance in the right part. m is for the distnaces inside the rectangle
	d1,d2,d3 are used to compare distances in the case of 3 points (brute force) */
	int medyan, i,j,k,n,count; /*medyan is the middle value of an array. i,j,k: used to iterate through
	the loops and arryas. n is used to compute the size of the array . count is used to find number of points in an array*/
	Nokta a2 ,b2; /*will be used to save coordinates of the closest Pair of Points in the right part*/
	n= finish-start+1; /**/
	if (n <= 3){ /* Brute Force Cases*/	
			if (n == 2 ){
				*a = arr[start]; /*saving start point */
				*b = arr[start+1]; /*saving finish point */
				dmin=  mesafe(arr[start],arr[start+1]);
				printf("\nThe shortest distance found so far is : %f\nIt is between (%d,%d) and (%d,%d)",dmin,a->x,a->y,b->x,b->y);
				return dmin;
			}
			else if(n == 3){
				d1 = mesafe(arr[start],arr[start+1]);
				d2= mesafe(arr[start],arr[finish]);
				d3= mesafe(arr[start+1],arr[finish]);
				if(d1<d2){
					if(d1<d3){
						*a = arr[start]; /*saving start point */
						*b = arr[start+1]; /*saving finish point */
						printf("\nThe shortest distance found so far is : %f\nIt is between (%d,%d) and (%d,%d)",d1,a->x,a->y,b->x,b->y);
						return d1;	
					}
					else{
						*a = arr[start+1]; /*saving start point */
						*b =  arr[finish]; /*saving finish point */
						printf("\nThe shortest distance found so far is : %f\nIt is between (%d,%d) and (%d,%d)",d3,a->x,a->y,b->x,b->y);
						return d3;
					}
				}
				else{
					if(d2<d3){
						*a = arr[start]; /*saving start point */
						*b = arr[finish]; /*saving finish point */
						printf("\nThe shortest distance found so far is : %f\nIt is between (%d,%d) and (%d,%d)",d2,a->x,a->y,b->x,b->y);
						return d2;	
					}
					else{
						*a = arr[start+1]; /*saving start point */
						*b =  arr[finish]; /*saving finish point */
						printf("\nThe shortest distance found so far is : %f\nIt is between (%d,%d) and (%d,%d)",d3,a->x,a->y,b->x,b->y);
						return d3;
					}
				}
			}
		}
		
	
	medyan = (finish+start)/2;
	dl= divide(arr,start,medyan, a,b);
	dr= divide(arr, medyan+1, finish,&a2,&b2);
	if(dl<dr){
		dmin= dl;
	}
	else{
		/**/
		dmin= dr;
		*a= a2; *b=b2;
	}
	
	/* Finding points that are close to the line*/
	Nokta line[n]; k=-1;
    for(i = start; i <= finish; i++){
    	if (abs(arr[i].x - arr[medyan].x) <= dmin){
        	k++;
			line[k].x = arr[i].x;
        	line[k].y = arr[i].y;	
		}
	}
        
	/*check the points within the rectangle*/
	mergeSort(line, 0, k , 0); /*we sort the points according to y coordinates vertically*/
	for(i=0; i<= k; i++){
		j= i+1;
		count=0;
		while(j<=k && count <6 ){ /*Because at most there can  be only 6 points in the rectangle*/
			/*We found a point inside the rectangle so compare it with the minimum*/
			if(abs(line[i].y - line[j].y)<=dmin){
				count++;
				m= mesafe(line[i],line[j]);
				if(m < dmin){
					dmin = m;
					*a= line[i];
					*b =line[j];
				}	
			}
			j++;	
		}
	}
	printf("\nThe shortest distance found so far is : %f\nIt is between (%d,%d) and (%d,%d)",dmin,a->x,a->y,b->x,b->y); 
    return dmin;
}

void merge(Nokta arr[], int start, int medyan, int finish,int x)
{
    int i, j, k;
    int n1 = medyan - start + 1;
    int n2 = finish - medyan;
  
    /* create temp arrays */
    Nokta L[n1], R[n2];
  
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++){
    	L[i].x = arr[start + i].x;
        L[i].y = arr[start + i].y;
	}
        
    for (j = 0; j < n2; j++){
    	R[j].x = arr[medyan + 1 + j].x;
        R[j].y = arr[medyan + 1 + j].y;
	}
        
  
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; /* Initial index of first subarray*/
    j = 0; /* Initial index of second subarray*/
    k = start; /* Initial index of merged subarray*/
    if(x==1){ /*If sorting will be according to x axis */
		while (i < n1 && j < n2) {
	    	
				if (L[i].x <= R[j].x ) {
	            arr[k].x = L[i].x;
	            arr[k].y = L[i].y;
	            i++;
		        }
		        else {
		            arr[k].x = R[j].x;
		            arr[k].y = R[j].y;
		            j++;
		        }
		        k++;

	    }
	}
	else{ /*Sorting will be according to x axis */
		while (i < n1 && j < n2) {
	    	
				if (L[i].y <= R[j].y ) {
	            arr[k].x = L[i].x;
	            arr[k].y = L[i].y;
	            i++;
		        }
		        else {
		            arr[k].x = R[j].x;
		            arr[k].y = R[j].y;
		            j++;
		        }
		        k++;
	       
	    }
		
	}
    /* Copy the remaining elements of L[] */
    while (i < n1) {
        arr[k].x = L[i].x;
        arr[k].y = L[i].y;
        i++;
        k++;
    }
  
    /* Copy the remaining elements of R[] */
    while (j < n2) {
        arr[k].x = R[j].x;
        arr[k].y = R[j].y;
        j++;
        k++;
    }
}

void mergeSort(Nokta arr[], int start, int finish, int x)
{
    if (start < finish) {
        int medyan = (start + finish) / 2;
  
        /* Sort first and second halves*/
        mergeSort(arr, start,medyan,x);
        mergeSort(arr, medyan + 1, finish,x);
  
        merge(arr, start, medyan, finish,x);
    }
}
void readFile(char * filename, Nokta* arr, int* size ){
	char c, coord [5]= {0}; 
	int i=0,j=0, k=0;
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL){
		printf("The file does not exist");
		return 1;
	}
    
	while((c = fgetc(fp)) != EOF) 
	{ 
		if(c == ' ') 
		{ 
			
			sscanf(coord, "%d", &(arr[k].x));
			for (i = 0; i < 5; ++i) 
			{
			    coord[i] = 0;
			}	
			j=0;
		}
		else if (c == '\n') 
		{ 
			sscanf(coord, "%d", &(arr[k].y));
			for (i = 0; i < 5; ++i) 
			{
			    coord[i] = 0;
			}
			j=0;
			k++;
		}
		else 
		{ 
			coord[j]= c;
			j++;
		} 
	} 
	sscanf(coord, "%d", &(arr[k].y));
    *size=k+1;
	fclose(fp); 
}
float mesafe(Nokta p1, Nokta p2){
    return sqrt( (p1.x - p2.x)*(p1.x - p2.x) +
                 (p1.y - p2.y)*(p1.y - p2.y)
               );
}

void printStruct(Nokta*  arr, int n){
	int i= 0;
	for(i=0;i<n; i++){
		printf("(%d,%d)\n",arr[i].x,arr[i].y);
	}
	
}

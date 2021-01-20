/* 
 * 
 * This code calculates the house price of a house by learing from
 * training data. It uses pseudo inverse of a given matrix to find the 
 * weight of different features.
 * 
 * Predicted Price : Y = W0 + W1*x1 + W2*X2 + W3*X3 + W4*X4
 * Weight Matrix : W = pseudoInv(X)*Y
 * pseudoInv(X) = inverse(transpose(X)*X) * transpose(X)  
 * 
 * weight(w) = pseudoInv(X) * Y
 * 			where	X = Input data matrix
 * 					Y = Target vector
 * 
 */
 
#include<stdio.h>
#include<stdlib.h>

// all methods declarations
double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2);
double** transposeMatrix(double** mat, int row, int col);
double** inverseMatrix(double **matA, int dimension);
void freeMatrix(double** mat, int row, int col); 
void printMatrix(double** mat, int row, int col);
void printArray(double* arr, int length);
double** calculateWeight(double** matTrain, double** matPrice, int trainRow, int trainCol);
// main method starts here
int main(int argc, char** argv){



	FILE* fp = fopen(argv[1], "r");
	
	int trainCol;
	int trainRow;
	
	fscanf(fp, "%d\n%d\n", &trainCol, &trainRow);
	
//	printf("%d: trainingCol, %d: trainingRow\n", trainCol, trainRow);
	
	double** trainMat = malloc(trainRow*sizeof(double*));
	double** trainPrice = malloc(trainRow*sizeof(double));
	//creates training Matrix
	for (int i = 0; i < trainRow; i++){
		trainMat[i] = malloc((trainCol+1)*sizeof(double));
		trainPrice[i] = malloc(sizeof(double));
	}	
	
	for (int i  = 0; i < trainRow; i++){
		for (int j = 0; j < trainCol+1; j++){
			if (j == 0)
				trainMat[i][j] = 1;
			else if  (j == trainCol){
				fscanf(fp, "%lf,%lf", &trainMat[i][j], &trainPrice[i][0]);
			}
			else {
				fscanf(fp, "%lf,", &trainMat[i][j]);		
			
			}
		}
	}
	fclose(fp);
	double** trainW = calculateWeight(trainMat, trainPrice, trainRow, trainCol);
	//printMatrix(trainW, trainCol+1, 1);
	
	FILE* fp2 = fopen(argv[2], "r");

	int houses;
	fscanf(fp2, "%d\n", &houses);
//	printf("%d\n", houses);

	double** testMat = malloc(houses*sizeof(double*));
	for (int i = 0; i < houses; i++){
		testMat[i] = malloc((trainCol+1)*sizeof(double));
	}

	for (int i = 0; i < houses; i++){
		for (int j = 0; j < trainCol+1; j++){
			if (j == 0)
				testMat[i][j] = 1;
			else {
				fscanf(fp2, "%lf,", &testMat[i][j]);
			}
			//printf("stuck");
		}
	}

//	printMatrix(testMat, houses, trainCol+1);
	double** final = multiplyMatrix(testMat, trainW, houses, trainCol+1, trainCol+1, 1);
	printMatrix(final, houses, 1);

	//free everything	
	freeMatrix(trainMat, trainRow, trainCol+1);
	freeMatrix(trainPrice, trainRow, 1);
	freeMatrix(trainW, trainCol+1, 1);
	freeMatrix(testMat, houses, trainCol+1);
	freeMatrix(final, houses, 1);
	fclose(fp2);
	return 0;
}

double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2)
{
	if (c1 != r2) printf("ERROR\n ERROR\n ERROR\n");	
	
	double** result=malloc(r1*sizeof(double*)); 
	for (int i = 0; i < r1; i ++){
		result[i] = malloc(c2*sizeof(double));
	}
    	for (int i = 0; i < r1; i++){
		for (int j = 0; j < c2; j++){
			result[i][j] = 0;
			for (int k = 0; k < r2; k++){
				result[i][j] += matA[i][k] * matB[k][j]; 
			}		
		
		}
	}
    	
	return result;
}


double** transposeMatrix(double** mat, int row, int col)
{
  
	double** matTran=malloc(col*sizeof(double*)); 
    	for (int i = 0; i < col; i++){
			matTran[i] = malloc(row*sizeof(double));
	}
	
	for (int i = 0; i < col; i++){
		for (int j = 0; j < row; j++){
			matTran[i][j] = mat[j][i];
		}
	}
    

	return matTran;        
}

double** inverseMatrix(double **matA, int dimension)
{
	//creates identity matrix
	double** matI=malloc(dimension*sizeof(double*)); 
	for (int i = 0; i < dimension; i++){
		matI[i] = malloc(dimension*sizeof(double));
		for (int  j = 0; j < dimension; j++){
			matI[i][j] = 0.0;
		}
		matI[i][i] = 1.0;
	}
    //invert
	for (int i = 0; i < dimension; i++){
		double f = matA[i][i];
		for (int d = 0; d < dimension; d++){
		//divide matA[i] by f
			matA[i][d] = matA[i][d] / f;	
		//divide matI[i] by f
			matI[i][d] = matI[i][d] / f;
		}
		for (int j = i+1; j < dimension; j++){
			double g = matA[j][i];
			//subtract matA[i] *  g from MatA[j]
			for (int k = 0; k < dimension; k++){
				matA[j][k] = matA[j][k] - (matA[i][k] * g);
			
			//subtract matI[i] * g from matI[i]
				matI[j][k] = matI[j][k] - (matI[i][k] * g);
			}
		}
	}

	for (int i = dimension - 1; i >= 0; i--){
		for (int j = i - 1; j >= 0; j--){
			double f = matA[j][i];
			for (int k = 0; k < dimension; k++){
			//subtract matA[i] * f from matA[j];
				matA[j][k] = matA[j][k] - (matA[i][k] * f); 
			//subtract matI[i] * f from matI[j];
				matI[j][k] = matI[j][k] - (matI[i][k] * f);
			}
		}

	}


	return matI;
}

void freeMatrix(double** mat, int row, int col){

	for (int i = 0; i < row; i++){
			free(mat[i]);
	}
	free(mat);	
}

void printMatrix(double** mat, int row, int col){

	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			printf("%0.0lf\t", mat[i][j]);
		}
		printf("\n");
	}

}
void printArray(double* arr, int length){

	for (int i = 0; i < length; i++){
		printf("%f\n", arr[i]);
	}
}

double** calculateWeight(double** trainMat, double** trainPrice, int trainRow, int trainCol){

	double** trainTrans = transposeMatrix(trainMat, trainRow, trainCol+1);
	//double** trainTrans_2 = transposeMatrix(trainMat, trainRow, trainCol+1);
 	double** trainMultiply = multiplyMatrix(trainTrans, trainMat, trainCol+1, trainRow, trainRow, trainCol+1);
 	double** trainInverse = inverseMatrix(trainMultiply, trainCol+1);
	double** trainMultiply_2 = multiplyMatrix(trainInverse, trainTrans, trainCol+1, trainCol+1, trainCol+1, trainRow);
 	double** trainW = multiplyMatrix(trainMultiply_2, trainPrice, trainCol+1, trainRow, trainRow, 1);

	freeMatrix(trainTrans, trainCol+1, trainRow);	
//      freeMatrix(trainTrans_2, trainCol+1, trainRow);
	freeMatrix(trainMultiply, trainCol+1, trainCol+1);
	freeMatrix(trainInverse, trainCol+1, trainCol+1);
 	freeMatrix(trainMultiply_2, trainCol+1, trainCol+1);
	
	return trainW;
}

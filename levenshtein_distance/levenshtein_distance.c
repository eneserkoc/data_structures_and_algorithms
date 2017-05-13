/** 


* @author

Name 			 : Enes Erkoç
Operating System : Ubuntu 16.04
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 1000

typedef struct QUEUE
{
	int front,		//kuyruk dizisi baÅŸ indis
		rear;		//kuyruk dizisi son indis
	int qu[SIZE];	//kuyruk dizisi
	int count;		//kuyruk dizisi eleman sayisi
}QUEUE;

int calculateWordNumber(FILE *kelime, int letterNumber);
char **createStringArray(FILE *kelime, int letterNumber, int wordNumber);
int **createGraph(char **stringArray, int wordNumber, int letterNumber);
int compareStrings(char *string1, char *string2, int letterNumber);
int findIndex(FILE *kelime, char *string, int letterNumber);
int doesStringExist(FILE *kelime, char *string, int letterNumber);

void initialize(QUEUE *queue);
int isEmpty(QUEUE *queue);
int isFull(QUEUE *queue);
void enqueue(int value, QUEUE *queue);
int dequeue(QUEUE *queue);

int main()
{
	FILE *kelime;
	
	int letterNumber,		//Kelimelerdeki harf sayisi
		exit = 0;			//Switch case dongu cikis degeri

	int selection;			//Switch case degiskeni

	int i,					
		index1,index2,		//Parametre olarak verilen string lerin stringArray icerisindeki indisleri
		counter = 0; 		//Adim sayisi degiskeni

	//Dosyada gecen kelimelerin harf sayisi kullanicidan alinir
	printf("\nKelime harf sayisi : "); scanf("%d", &letterNumber);
	//Switch case icindeki seceneklerde kullanicidan alinan string leri tutmak icin kullanilan buffer lar
	char *buffer1 = (char *)malloc(letterNumber * sizeof(char));
	char *buffer2 = (char *)malloc(letterNumber * sizeof(char));

	kelime = fopen("kelime.txt", "r");
	if(kelime == NULL){
		printf("Dosya acilamadi");
	}

	int wordNumber = calculateWordNumber(kelime, letterNumber);

	char **stringArray = createStringArray(kelime, letterNumber, wordNumber);

	int **graph = createGraph(stringArray, wordNumber, letterNumber);

	while(!exit)
	{
		printf("\n 1 - Tek harf degisimi kontrolu\n");
		printf("\n 2 - Iki kelime arasindaki donusum kontrolu\n");
		printf("\n 0 - Cikis\n ");
		printf("\nYapmak istediginiz islemi seciniz : "); scanf("%d", &selection);

		switch(selection)
		{
			case 1 : 	printf("\n1. String : "); scanf("%s", buffer1);
						printf("\n2.String : "); scanf("%s", buffer2);
	
						if(doesStringExist(kelime, buffer1, letterNumber))
						{
							if(doesStringExist(kelime, buffer2, letterNumber))
							{
								index1 = findIndex(kelime, buffer1, letterNumber);
								index2 = findIndex(kelime, buffer2, letterNumber);

								//Bulunan indislere karsilik gelen matris gozu 1'se tek harf degisimi var
								if(graph[index1][index2])
								{
									printf("\nTek harf degisimi var...\n");
								}
								else
								{
									printf("\nTek harf degisimi yok...\n");
								}
							}
							else
							{
								printf("\n' %s ' bulunamadi...\n", buffer2);
							}
						}
						else
						{
							printf("\n' %s ' bulunamadi...\n", buffer1);
						}
			break;

			case 2 : 	printf("\n1. String : "); scanf("%s", buffer1);
						printf("\n2.String : "); scanf("%s", buffer2);
						
						if(doesStringExist(kelime, buffer1, letterNumber))
						{
							if(doesStringExist(kelime, buffer2, letterNumber))
							{
								QUEUE *queue = (QUEUE *)malloc(sizeof(QUEUE));
								if(queue == NULL){
									printf("Not allocated...\n");
								}
								initialize(queue);	//Olusturulan queue nin ilk deger atamalari yapiliyor.

								int *additionalList = (int *)calloc(wordNumber, sizeof(int));
								if(additionalList == NULL){
									printf("Not allocated...\n");
								}

								int *parent = (int *)calloc(wordNumber, sizeof(int));
								if(parent == NULL){
									printf("Not allocated...\n");
								}

								index1 = findIndex(kelime, buffer1, letterNumber);
								index2 = findIndex(kelime, buffer2, letterNumber);


								enqueue(index1, queue); //ilk string in index i queue ye eklenir
								additionalList[index1] = 1; //queue ye eklendigi icin daha once eklendiginin bilgisi additionalList de tutulur.
								
								//Dongu cikis dugumune ulasana kadar veya kuyruk bosalana kadar devam eder.								
								printf("\nKuyruktan Cekilenler : ");
								while( (queue->qu[queue->front] != index2) && (!isEmpty(queue)) )
								{
									//Kuyruktan cikacak olan dugumdeki kelime yazdirilir
									printf("%s -> ", stringArray[queue->qu[queue->front]]);
									//Kuyruktan kac kelimenin ciktigini bulmak adina sayac tutulur.
									counter++; 
									//Kuyruktan cikacak olan kelimenin komsulari queue ye eklenir.
									//Eklenecek kelimelerin daha once eklenmemis olmasi kontrol edilir
									//Bu kontrol additionalList dizisine her eklenen kelimenin indisindeki goze 1 yazarak saglanir
									//Boylece eger goz 1 se o kelimenin daha once kuyruga girdigi anlasilir.
									for(i=0; i<wordNumber; i++)
									{
										if(graph[queue->qu[queue->front]][i]) //or graph[index][i] == 1
										{
											if(additionalList[i] != 1)
											{
												enqueue(i, queue);
												additionalList[i] = 1;
												//parent dizisi sayesinde kuyruga ekledigimiz kelimelerin hangi kelimelerin komsusu olarak eklendigini gorebilicez
												//ex : parent[4] = 5 , buradan 4. kelimenin 5. kelimenin komsulugundan queue ye eklendigini anliyoruz.
												parent[i] = queue->qu[queue->front];
											}
										}
									}

									dequeue(queue); //aslinda queue den cikarilan degeri donduruyor ama onla bir isimiz yok sadece cikarmasi yeterli
								}

								if(!isEmpty(queue))
								{
									//kelimeye ulasildiysa kuyrugun en basindadir ama donguden cikildigi icin yazdirilmamistir
									//kontrol yapildiktan sonra o da yazdirilir.
									printf("%s", stringArray[queue->qu[queue->front]]); 
									printf("\n\nDonusum var ... %d Adim var\n\n", counter);

									
									printf("Harf Degisimi Yolu(TERSTEN) : ");
									//Kuyrugu yazdirirken son kaldigimiz indisten itibaren geriye dogru gideriz
									//Burdaki amac ulastigimiz kelimenin(buffer2) hangi kelimelerin komsulugundan turedigini bulmak
									//Bunu gerceklestirebilmek adina onceden parent dizisini tutmustuk
									counter = 0; //Adim sayisi icin kullanmistik simdi harf degisimi sayisi icin kullanicaz
									i = queue->qu[queue->front];
									while(i != index1)
									{
										printf("%s <- ", stringArray[i]);
										i = parent[i];
										counter++;
									}
									printf("%s", stringArray[i]);
									printf("\n\n%d Harf degisimi var...\n", counter);
								}
								else
								{
									printf("\n\nDonusum yok...\n");
								}

								counter = 0; //daha sonraki orneklerde kullanilmasi adina sifirlanir.
								free(queue);
								free(additionalList);
								free(parent);

							}
							else
							{
								printf("\n' %s ' bulunamadi...\n", buffer2);
							}
						}
						else
						{
							printf("\n' %s ' bulunamadi...\n", buffer1);
						}
			break;

			case 0 : 	exit = 1;
						printf("\nProgramdan cikis yaptiniz...\n\n");
			break;

			default : printf("\nYanlis bir secim girdiniz...Tekrar giriniz\n");
		}
	}

	free(kelime);
	free(stringArray);
	free(graph);

	return 0;
}
/** Verilen dosyadaki kelime sayisini bulur
	
	@param		kelime 					: File pointer			
	@param		letterNumber			: Kelimelerin harf sayisi
	@return 	i						: Kelime sayisi
*/
int calculateWordNumber(FILE *kelime ,int letterNumber)
{
	char *buffer = (char *)malloc(letterNumber * sizeof(char));
	int i=0;

	while(!feof(kelime))
	{
		fscanf(kelime,"%s", buffer);
		i++;
	}

	rewind(kelime);

	return i;
}
/** Verilen dosyadaki kelimeleri diziye kopyalar
	
	@param		kelime 					: File pointer			
	@param		letterNumber			: Kelimelerin harf sayisi
	@param		wordNumber 				: Kelime sayisi
	@return 	stringArray				: String dizisi adres
*/
char **createStringArray(FILE *kelime, int letterNumber, int wordNumber)
{
	int i;

	char **stringArray = (char **)malloc(wordNumber * sizeof(char *));

	for(i=0; i<wordNumber; i++)
	{
		stringArray[i] = (char *)malloc((letterNumber+1) * sizeof(char)); //kelimelerdeki harf sayisi 5
	}

	i=0;
	while(!feof(kelime))
	{
		fscanf(kelime,"%s", stringArray[i]);
		i++;
	}

	rewind(kelime);

	return stringArray;
}
/** Verilen iki string harflerini karsilastirip farklilik sayisini bulur
			
	@param 	 	string1				: 1. String
	@param		string2				: 2.String
	@param		letterNumber 		: Kelimelerin harf sayisi
	@return		count 				: Farkli harflerin sayisi
*/
int compareStrings(char *string1, char *string2, int letterNumber)
{
	int i,
		count=0;

	for(i=0; i<letterNumber; i++)
	{
		if(string1[i] == string2[i])
		{
			count++;
		}
	}

	return count;
}
/** Verilen string dizisinde aralarinda tek harf donusumu olan kelimeleri gosteren bir matris(graf) uretir
			
	@param 	 	stringArray				: String dizisi adres
	@param		letterNumber			: Kelimelerin harf sayisi
	@param		wordNumber 				: Kelime sayisi
	@return		graph 					: Matris(graf) adres
*/
int **createGraph(char **stringArray, int wordNumber, int letterNumber)
{
	int i,
		j;

	int **graph = (int **)malloc(wordNumber * sizeof(int *));

	if(graph == NULL)
	{
		printf("Not allocated...\n");
	}

	for(i=0; i<wordNumber; i++)
	{
		graph[i] = (int *)malloc(wordNumber * sizeof(int *));

		if(graph[i] == NULL)
		{
			printf("Not allocated...\n");
		}
	}

	//string ler karsilastirip 1 ve 0 degerleri ataniyor aradaki iliskilere gore
	for(i=0; i<wordNumber-1; i++)
	{
		for(j=i+1; j<wordNumber; j++)
		{
			if(compareStrings(stringArray[i], stringArray[j], letterNumber) == letterNumber-1)
			{
				graph[i][j] = graph[j][i] = 1;
			}
		}
	}

	return graph;
}
/** Verilen dosya icerisinde verilen string in kacinci sirada oldugunu bulur
			
	@param		kelime 					: File pointer	
	@param		string 					: Sirasi bulunmak istenen string
	@param		letterNumber 			: Kelimelerin harf sayisi
	@return		index 					: Sira bilgisi
*/
int findIndex(FILE *kelime, char *string, int letterNumber)
{
	
	int index=0;

	char *buffer = (char *)malloc(letterNumber * sizeof(char));
	
	fscanf(kelime, "%s", buffer);

	while((!feof(kelime)) && (strcmp(string, buffer) != 0))
	{
		fscanf(kelime, "%s", buffer);
		index++;
	}
	rewind(kelime);

	//Eger kelime bulunabildiyse
	if(strcmp(string, buffer) != 0)
	{
		index = -1;
	}
	
	return index;
}
/** Verilen dosya icerisinde kelimenin bulunup bulunmadigini kontrol eder
			
	@param		kelime 					: File pointer	
	@param		string 					: Aranan kelime
	@param		letterNumber 			: Kelimelerin harf sayisi
	@return		check 					: Bulunup bulunmadigini gosteren degisken
*/
int doesStringExist(FILE *kelime, char *string, int letterNumber)
{
	int check = 0;

	int index = findIndex(kelime, string, letterNumber);

	rewind(kelime);

	if(index != -1)
	{
		check = 1;
	}

	return check;
}

/* QUEUE FUNCTIONS*/

void initialize(QUEUE *queue)
{
	queue->front = queue->rear = 0; //kuyrukta bir sey olmadigi icin baslangicta
	queue->count = 0; 
}
int isEmpty(QUEUE *queue)
{
	if(queue->count == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int isFull(QUEUE *queue)
{
	if(queue->count == SIZE)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void enqueue(int value, QUEUE *queue)
{
	if(isFull(queue))
	{
		printf("Queue is full\n");
	}
	else
	{
		queue->qu[queue->rear] = value;
		queue->rear = ((queue->rear)+1) % SIZE; // basa donebilmek icin dairesel yapi
		(queue->count)++;
	}
}

int dequeue(QUEUE *queue)
{
	int item;

	if(isEmpty(queue))
	{
		printf("Queue is Empty...\n");

		return -1;
	}
	else
	{
		item = queue->qu[queue->front]; 
		//Eger item i tanimlamamis olsaydik queue->front degerini arttirdiktan sonra tekrar azaltmamiz gerekecekti
		queue->front = (queue->front + 1) % SIZE;
		(queue->count)--;		
		return item;
	}
}

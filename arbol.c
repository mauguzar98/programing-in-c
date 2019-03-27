#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Def_Dato
{
  char simbolo;
  float prob;
  struct Def_Dato  *sig, *der, *izq;
}tipodato;

void Ingresar(tipodato **Inicio, float *Suma);
void Listar(tipodato *Inicio);
void ListarArbol(tipodato *Raiz);
void Eliminar(tipodato **Inicio, float *Suma);
void Modificar(tipodato **Inicio, float *Suma);
void GenerarArbol(tipodato **Inicio, tipodato **Raiz);
void Codificar(tipodato *Raiz, char Mensaje[100], int i, int j, char Cod[100], char NombreArchivo[20]);
void Decodificar(tipodato *Raiz);
void BorrarLista(tipodato *Inicio);
void BorrarArbol(tipodato *Raiz);

int main (void)
{
  int Opc, j=0, i=0;
  float Suma=0;
  char NomArch1[50], Mensaje[100], Cod[100],NombreArchivo[20];
  FILE *Archivo;
  tipodato *Raiz=NULL, *Inicio=NULL;
  system("clear");
  //printf("Desarrollado por:\nDalia Braverman Jaiven\nJuan Jose Aguero Vargas\n");
  getchar();
  printf("Este programa desarrolla los codigos de longitud variable a partir de simbolos y sus probabilidades de uso ingresados por el usuario\n\n");
  printf("Presione <ENTER> para iniciar\n");
  getchar();
  system("clear");
  do
    {
      printf ("Menu:\n1. Ingresar\n2. Listar\n3. Eliminar\n4. Modificar\n5. Generar codigo\n6. Codificar\n7. Decodificar\n8. Salir\n");
      scanf ("%d",&Opc);
      switch (Opc)
	{
	case 1:
	  Ingresar(&Inicio,&Suma);
	  printf("La suma de las probabilidades es: %.2f\n", Suma);
	  break;
	case 2:
	  if (Raiz==NULL)
	    Listar(Inicio);
	  else
	    ListarArbol(Raiz);
	  break;
	case 3:
	  if (Raiz==NULL) //valida que no se haya creado el arbol
	    Eliminar (&Inicio, &Suma);
	  else
	    printf ("Una vez generado el arbol no se pueden hacer cambios\n");
	  break;
	case 4:
	  if (Raiz==NULL) //valida que no se haya creado el arbol
	    Modificar (&Inicio, &Suma);
	  else
	    printf ("Una vez generado el arbol, no se pueden hacer cambios\n");
	case 5:
	  if(Suma==100)
	    {
	      GenerarArbol (&Inicio, &Raiz);
	      printf("Codigos generados correctamente\n");
	    }
	  else
	    printf("La suma de las probabilidades no es 100 por lo que no puedo generar codigos\n");
	  break;
	case 6:
	  if (Raiz!=NULL) //valida que el arbol ya este creado
	    {
	      printf ("Escribe el nombre del archivo donde se encuentra el mensaje que deseas codificar (con extension)\n");
	      fpurge(stdin);
	      gets (NomArch1);
	      Archivo=fopen(NomArch1,"rt");
	      if (Archivo!=NULL)
		{
		  fscanf (Archivo, "%s",Mensaje);
		  fclose (Archivo);
		  for(j=0; j<strlen(Mensaje); j++)
		    Codificar (Raiz,Mensaje,i, j, Cod, NombreArchivo);
		}
	      else
		printf ("EL archivo no existe, intentelo de nuevo\n");
	    }
	  else
	    printf ("Antes de codificar, debera crear el arbol, para hacerlo elija la opcion 5 del siguiente menu\n");
	  break;
	case 7:
	  if (Raiz!=NULL) //valida que el arbol ya este creado
	    {
	      Decodificar(Raiz);
	      printf("Mensaje decodificado correctamente\n");
	    }
	  else
	    printf ("No se puede decodificar sin haber creado previamente el arbol, seleccione la opcion 5 del siguiente menu para hacerlo\n");
	  break;
	case 8:
	  if (Inicio!=NULL)
	    BorrarLista (Inicio);
	  if (Raiz!=NULL)
	    BorrarArbol (Raiz);
	  break;
	default:
	  printf("No existe esa opcion intenta de nuevo\n\n");
	  break;
	}
    }while (Opc!=8);
}

void Ingresar (tipodato **Inicio, float *Suma)
{
  char Simbolo;
  tipodato *Repetido=*Inicio, *temp, *pos, *temp2;

  printf ("Simbolo: ");
  fpurge(stdin);
  scanf ("%c",&Simbolo);
  while ((Repetido!=NULL) && (Repetido->simbolo!=Simbolo)) //validamos que el simbolo que va a insertar no se repita
    Repetido=Repetido->sig;
  if (Repetido!=NULL)
    printf ("El simbolo ya existe\n\n");
  else
    {
      temp=(tipodato*)malloc(sizeof (tipodato));
      temp->simbolo= Simbolo;
      printf ("Probabilidad: ");
      fpurge(stdin);
      scanf ("%f", &temp->prob);
      *Suma=*Suma + temp->prob;
      if (*Suma<=100)
	{
	  if (*Inicio==NULL)
	    {
	      temp->sig=NULL;
	      *Inicio=temp;
	    }
	  else
	    {
	      pos=*Inicio;
	      while((pos!=NULL) && (pos->prob<temp->prob)) //encontramos posicion para insertarlo en orden
		pos=pos->sig;
	      if(pos==*Inicio) //inserta al inicio
		{
		  temp->sig=*Inicio;
		  *Inicio=temp;
		}
	      else
		{
		  if(pos==NULL) //inserta al final
		    {
		      temp2=*Inicio;
		      while(temp2->sig!=NULL)
			temp2=temp2->sig;
		      temp->sig=NULL;
		      temp2->sig=temp;
		    }
		  else
		    {
		      if(pos!=*Inicio) //inserta en medio
			{
			  temp2=*Inicio;
			  while(temp2->sig!=pos)
			    temp2=temp2->sig;
			  temp->sig=pos;
			  temp2->sig=temp;
			}
		    }
		}
	    }
	  if(*Suma==100)
	    printf("La suma de las probabilidades ya es 100\n\n");
	}
      else //no permite al usuario ingresar un nuevo simbolo si la suma de las probabilidades es mayor a 100
	{
	  printf("Las probabilidades suman mas de 100 por lo que no puedo insertar un nuevo simbolo\n\n");
	  *Suma=*Suma-temp->prob;
	}
    }
}

void Listar(tipodato *Inicio)
{
  tipodato *temp=Inicio;
  while(temp!=NULL)
    {
      printf("%c-%.2f\n", temp->simbolo, temp->prob);
      temp=temp->sig;
    }
}

void ListarArbol(tipodato *Raiz)
{
  if(Raiz!=NULL)
    {
      ListarArbol(Raiz->izq);
      if((Raiz->der!=NULL) && (Raiz->izq!=NULL))
      printf("%c-%.2f\n", Raiz->simbolo, Raiz->prob);
      ListarArbol(Raiz->der);
    }
}

void Eliminar(tipodato **Inicio, float *Suma)
{
  char eliminar;
  tipodato *temp, *temp2;
  if(Inicio!=NULL)
    {
      printf("Escribe el simbolo que deseas eliminar\n");
      fpurge(stdin);
      scanf("%c", &eliminar);
      temp=*Inicio;
      while(temp->simbolo!=eliminar) //encuentra el simbolo a eliminar
	temp=temp->sig;
      if(temp==*Inicio) //conecta la lista correctamente antes de eliminar por completo el simbolo
	{
	  *Inicio=temp->sig;
	  *Suma=*Suma-temp->prob;
	  free(temp);
	  printf("El simbolo fue eliminado correctamente y ahora la suma es %.2f. Recuerda que debes ingresar o modificar algun simbolo para que la suma sea 100\n\n", *Suma);
	}
      else
	{
	  if(temp!=NULL)
	    {
	      temp2=*Inicio;
	      while(temp2->sig!=temp)
		temp2=temp2->sig;
	      temp2->sig=temp->sig;
	      *Suma=*Suma-temp->prob;
	      free(temp);
	      printf("El simbolo fue eliminado correctamente y ahora la suma es %.2f. Recuerda que debes ingresar o modificar algun simbolo para que la suma sea 100\n\n", *Suma);
	    }
	  else
	    {
	      printf("No existe el simbolo\n\n");
	    }
	}
    }
  else
    printf("No se puede eliminar una lista vacia\n");
}

void Modificar(tipodato **Inicio, float *Suma)
{
  int Opc;
  char modificar, nsimbolo;
  float nprob;
  tipodato *temp, *temp2, *pos, *temp3;
  printf("Menu:\n1. Modificar simbolo\n2. Modificar probabilidad\n");
  fpurge(stdin);
  scanf("%d", &Opc);
  printf("Escribe el simbolo de la casilla que deseas modificar\n");
  fpurge(stdin);
  scanf("%c", &modificar);
  temp=*Inicio;
  while(temp->simbolo!=modificar) //encuentra el simbolo que va a modificar
    temp=temp->sig;
  if(temp==NULL)
    printf("No existe ese simbolo\n\n");
  else
    {
      switch(Opc)
	{
	case 1:
	  printf("Escribe el nuevo simbolo\n");
	  fpurge(stdin);
	  scanf("%c", &nsimbolo);
	  temp->simbolo=nsimbolo; //se modifica el simbolo
	  printf("Simbolo modificado correctamente\n\n");
	  break;
	case 2:
	  printf("Escribe la nueva probabilidad:\n");
	  fpurge(stdin);
	  scanf("%f", &nprob);
	  *Suma=(*Suma)-(temp->prob)+nprob;
	  temp->prob=nprob; //se modifica la probabilidad despues saca el simbolo de la lista
	  if(temp==*Inicio)
	    {
	      *Inicio=temp->sig;
	      temp->sig=NULL;
	    }
	  else
	    {
	      if (temp->sig==NULL)
		{
		  temp2=*Inicio;
		  while (temp2->sig!=temp)
		    temp2=temp2->sig;
		  temp2->sig=NULL;
		}
	      else
		{
		  temp2=*Inicio;
		  while (temp2->sig!=temp)
		    temp2=temp2->sig;
		  temp3=temp->sig;
		  temp->sig=NULL;
		  temp2->sig=temp3;
		}
	    }
	  pos=*Inicio;
	  while ((pos!=NULL)&&(temp->prob>pos->prob)) //encuentra la posicion en la que debe de ser reinsertado para que este en orden
	    pos=pos->sig;
	  if (pos==*Inicio)
	    {
	      temp->sig=*Inicio;
	      *Inicio=temp;
	    }
	  else
	    {
	      if (pos==NULL)
		{
		  temp2=*Inicio;
		  while (temp2->sig!=NULL)
		    temp2=temp2->sig;
		  temp2->sig=temp;
		}
	      else
		{
		  temp2=*Inicio;
		  while (temp2->sig!=pos)
		    temp2=temp2->sig;
		  temp->sig=pos;
		  temp2->sig=temp;
		}
	    }
	  printf("La suma ahora es %.2f, recuerda que debe de ser 100\n", *Suma);
	  break;
	}
    }
}

void GenerarArbol(tipodato **Inicio, tipodato **Raiz)
{
  tipodato *temp, *temp2, *temp3, *temp4, *pos;
  while(*Inicio!=NULL)
    {
      temp=*Inicio;
      *Inicio=(*Inicio)->sig;
      temp2=*Inicio;
      *Inicio=(*Inicio)->sig;
      temp3=(tipodato*)malloc(sizeof(tipodato));
      temp3->simbolo='='; //le agregamos este simbolo para que cuando compare no compare con basura
      temp3->prob=temp->prob+temp2->prob;
      temp3->izq=temp;
      temp3->der=temp2;
      if(temp->simbolo!='=')
	{
	  temp->der=NULL;
	  temp->izq=NULL;
	}
      temp->sig=NULL;
      if(temp2->simbolo!='=')
	{
	  temp2->der=NULL;
	  temp2->izq=NULL;
	}
      temp2->sig=NULL;
      if(temp3->prob==100) //el arbol acabo de crearse
	{
	  *Raiz=temp3;
	}
      else
	{
	  pos=*Inicio;
	  while((pos!=NULL) && (temp3->prob>pos->prob)) //ordena el nuevo nodo
	    {
	      pos=pos->sig;
	    }
	  if(pos==*Inicio)
	    {
	      temp3->sig=*Inicio;
	      *Inicio=temp3;
	    }
	  else
	    {
	      if(pos==NULL)
		{
		  temp4=*Inicio;
		  while(temp4->sig!=NULL)
		    temp4=temp4->sig;
		  temp3->sig=NULL;
		  temp4->sig=temp3;
		}
	      else
		{
		  if(pos!=*Inicio)
		    {
		      temp4=*Inicio;
		      while(temp4->sig!=pos)
			temp4=temp4->sig;
		      temp4->sig=temp3;
		      temp3->sig=pos;
		    }
		}
	    }
	}
    }
}

void Codificar(tipodato *Raiz, char Mensaje[100], int i, int j, char Cod [100],char NombreArchivo[20])
{
  FILE *Archivo;
  if(Raiz!=NULL)
    {
      if(Raiz->izq!=NULL) //Valida que pueda desplazarse a la izquierda y de ser así agrega un 0 al codigo
	{
	  Cod[i]='0';
	  Codificar(Raiz->izq, Mensaje,i+1,j, Cod, NombreArchivo);
	  Cod[i]='\0';
	}
      if(Raiz->der!=NULL) //Valida que pueda desplazarse a la derecha y de ser así agrega un 1 al codigo
	{
	  Cod[i]='1';
	  Codificar(Raiz->der, Mensaje, i+1,j, Cod,NombreArchivo);
	  Cod[i]='\0';
	}
      //Cod[i]='\0';
      if(Mensaje[j]==Raiz->simbolo) //Como ya se encuentra en una hoja, valida que el simbolo sea el siguiente en la frase leida
	{
	  Cod[i]='\0';
	  if (j==0)
	    {
	      printf ("Escribe el nombre del archivo donde quieres guardar el codigo (con extension):\n");
	      fpurge(stdin);
	      gets(NombreArchivo);
	    }
	  Archivo=fopen(NombreArchivo, "at");
	  fprintf(Archivo, "%s", Cod);
	  fclose(Archivo);
	}
    }
}

void Decodificar(tipodato *Raiz)
{
  char NomArch[50], Codigo[500], Frase[100], NomArch2[50];
  FILE *Archivo;
  tipodato *temp;
  int j, Bandera=0, i;
  printf ("Escribe el nombre del archivo donde se  encuentra el mensaje que quieres decodificar (con extension):\n");
  fpurge(stdin);
  gets (NomArch);
  strcat(NomArch,"txt");
  Archivo=fopen(NomArch,"rt");
  if (Archivo==NULL)
    {
      printf ("El archivo no existe\n");
    }
  else
    {
      fscanf(Archivo, "%s", Codigo);
      fclose (Archivo);
      //Codigo[strlen(Codigo)-1]='\0';
      j=0;
      temp=Raiz;
      for (i=0;i<strlen(Codigo);i++)
	{
	  if ((Codigo[i]=='0')||(Codigo[i]=='1')) //valida que el codigo sea binario
	    {
	      if (Codigo[i]=='0')
		{
		  temp=temp->izq; //si es 0 se recorre a la izquierda
		}
	      if (Codigo[i]=='1')
		{
		  temp=temp->der; //si es 1 se recorre a la derecha
		}
	      if ((temp->der==NULL)&&(temp->izq==NULL)) //llega a la hoja y imprime el simbolo en otro string
		{
		  Frase[j]=temp->simbolo;
		  j++;
		  temp=Raiz;
		}
	    }
	  else
	    {
	      printf ("El codigo del archivo no es binario, por lo tanto no se puede leer");
	      Bandera=1;
	    }
	}
      if (Bandera==0)
	{
	  printf ("Escribe el nombre del archivo donde deseas guardar la frase (con extension):\n");
	  fpurge (stdin);
	  gets (NomArch2);
	  Archivo=fopen (NomArch2, "wt");
	  fprintf (Archivo, "%s",Frase);
	  // fputs (Frase,Archivo); //imprime en el archivo el mensaje
	  fclose (Archivo);
	}
    }
}

void BorrarLista(tipodato *Inicio)
{
  tipodato *temp;
  temp=Inicio;
  while(temp!=NULL)
    {
      Inicio=Inicio->sig;
      free(temp);
      temp=Inicio;
    }
}

void BorrarArbol(tipodato *Raiz)
{
  if(Raiz!=NULL)
    {
      BorrarArbol(Raiz->izq);
      BorrarArbol(Raiz->der);
      free(Raiz);
    }
}

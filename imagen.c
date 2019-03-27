#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void Negativo();
void Ecualizar();
void Reducir();
void Ampliar();

int main(void)
{
    int x, y;
    int Opcion;
    system("clear");
    printf("Elaborado por: Mauricio.(Presiona ENTER para continuar)\n");
    getchar();
    printf("El objetivo del programa es darle al usuario la opcion de invertir los colores de una imagen, ecualizar una imagen, reducir o ampliar una imagen por medio de archivos y arreglos como herramienta principal.(Presiona ENTER para continuar)\n");
    getchar();
    system("clear");
    printf("\t\t\tMenu\nElige la opcion que desees realizar:\n\t1)Invertir colores de la imagen.\n\t2)Ecualizar la imagen.\n\t3)Reducir la imagen.\n\t4)Ampliar la imagen.\n");
    scanf("%d", &Opcion);
    switch(Opcion)
    {
        case 1:
            Negativo();
            break;
        case 2:
            Ecualizar();
            break;
        case 3:
            Reducir();
            break;
        case 4:
            Ampliar();
            break;
        default:
            printf("ERROR. Opcion invalida.\n");
            exit(0);
            break;
    }
}

void Negativo()
{
    unsigned char **Neg;
    FILE *Archivo, *Archivos;
    char NombreArchivo[20], Encabezado[16];
    int Filas=0, Columnas=0, i, j;
    fpurge(stdin);
    printf("Dame el nombre del archivo con extension (.pgm) que contiene la imagen a modificar: \n");
    gets(NombreArchivo);
    Archivo=fopen(NombreArchivo, "rb");
    if(Archivo==NULL)
    {
        printf("ERROR. El archivo no existe.\n");
        exit(0);
    }
    else
    {
        
        fread(&Encabezado, sizeof(char), 15, Archivo); //Lee el encabezado
        sscanf(Encabezado, "P5 %d %d 255 ", &Columnas, &Filas); //Lee el tamanio de la imagen
        Neg=(unsigned char**)malloc(sizeof(unsigned char*)*Filas);
        for(i=0; i<Filas; i++)
        {
            Neg[i]=(unsigned char*)malloc(sizeof(unsigned char)*Columnas);
        }
        
        for(i=0; i<Filas; i++)
        {
            for(j=0; j<Columnas; j++)
            {
                fread(&Neg[i][j], sizeof(unsigned char), 1, Archivo);
            }
        }
        
        for(i=0; i<Filas; i++)
        {
            for(j=0; j<Columnas; j++)
            {
                Neg[i][j]=Neg[i][j]*(-1)+255; //Invierte los colores de la imagen
            }
        }
        
        fclose(Archivo);
    }
    printf("Gracias.\n");
    printf("\n");
    fpurge(stdin);
    printf("Dame el nombre del archivo en el que deseas guardar la imagen modificada (sin extension): \n");
    gets(NombreArchivo);
    strcat(NombreArchivo, ".pgm");
    Archivos=fopen(NombreArchivo, "wt");
    fprintf(Archivos, "P5 %d %d 255 ", Columnas, Filas); //Copia al archivo nuevo el encabezado ya modificado
    for(i=0; i<Filas; i++)
    {
        for(j=0; j<Columnas; j++)
        {
            fprintf(Archivos, "%c", Neg[i][j]); //Copia al archivo el arreglo de la imagen ya modificado
        }
    }
    printf("La imagen se guardo de forma exitosa.\n");
    printf("\nPara ver la imagen modificada, escribe lo siguiente en la terminal:\n\t\temacs NombreDelArchivo.pgm\n");
    fclose(Archivos);
    for(i=0; i<Filas; i++) //Borra los malloc creados
        free(Neg[i]);
    free(Neg);
}

void Ecualizar()
{
    unsigned char **Ecu;
    FILE *Archivo, *Archivos;
    char NombreArchivo[20], Encabezado[16];
    int Filas=0, Columnas=0, i, j, Max=255, Min=0;
    fpurge(stdin);
    printf("Dame el nombre del archivo con extension (.pgm) que contiene la imagen a modificar: \n");
    gets(NombreArchivo);
    Archivo=fopen(NombreArchivo, "rb");
    if(Archivo==NULL)
    {
        printf("ERROR. El archivo no existe.\n");
        exit(0);
    }
    else
    {
        
        fread(&Encabezado, sizeof(char), 15, Archivo); //Lee el encabezado de la imagen
        sscanf(Encabezado, "P5 %d %d 255 ", &Columnas, &Filas); //Lee el tamanio de la imagen
        Ecu=(unsigned char**)malloc(sizeof(unsigned char*)*Filas);
        for(i=0; i<Filas; i++)
        {
            Ecu[i]=(unsigned char*)malloc(sizeof(unsigned char)*Columnas);
        }
        
        for(i=0; i<Filas; i++)
        {
            for(j=0; j<Columnas; j++)
            {
                fread(&Ecu[i][j], sizeof(unsigned char), 1, Archivo);
            }
        }
        for(i=0; i<Filas; i++)
        {
            for(j=0; j<Columnas; j++)
            {
                if(Ecu[i][j]<Min) //Lee el menor valor de la imagen
                    Min=Ecu[i][j];
                if(Ecu[i][j]>Max) //Lee el mayor valor de la imagen
                    Max=Ecu[i][j];
            }
        }
        
        for(i=0; i<Filas; i++)
        {
            for(j=0; j<Columnas; j++)
            {
                Ecu[i][j]=((255)*(Max-Min))*(Ecu[i][j]-Min); //Ecuacion para ecualizar la imagen
            }
        }
        
        fclose(Archivo);
    }
    printf("Gracias.\n");
    printf("\n");
    fpurge(stdin);
    printf("Dame el nombre del archivo en el que deseas guardar la imagen modificada (sin extension): \n");
    gets(NombreArchivo);
    strcat(NombreArchivo, ".pgm");
    Archivos=fopen(NombreArchivo, "wt");
    fprintf(Archivos, "P5 %d %d 255 ", Columnas, Filas); //Copia al archivo nuevo el encabezado ya modificado
    for(i=0; i<Filas; i++)
    {
        for(j=0; j<Columnas; j++)
        {
            fprintf(Archivos, "%c", Ecu[i][j]); //Copia al archivo el arreglo con la imagen modificada
        }
    }
    printf("El archivo se guardo de forma exitosa.\n");
    printf("\nPara ver la imagen modificada, escribe lo siguiente en la terminal:\n\t\temacs NombreDelArchivo.pgm\n");
    fclose(Archivos);
    for(i=0; i<Filas; i++) //Borra los malloc creados
        free(Ecu[i]);
    free(Ecu);
}


void Reducir()
{
    unsigned char **Redu, **Redu2;
    FILE *Archivo, *Archivos;
    char NombreArchivo[20], Encabezado[16];
    int Filas=0, Columnas=0, i, j, Cols=0, Fils=0;
    fpurge(stdin);
    printf("Dame el nombre del archivo con extension (.pgm) que contiene la imagen a modificar: \n");
    gets(NombreArchivo);
    Archivo=fopen(NombreArchivo, "rb");
    if(Archivo==NULL)
    {
        printf("ERROR. El archivo no existe.\n");
        exit(0);
    }
    else
    {
        
        fread(&Encabezado, sizeof(char), 15, Archivo); //Lee el encabezado de la imagen
        sscanf(Encabezado, "P5 %d %d 255 ", &Columnas, &Filas); //Lee el tamanio de la imagen
        Redu=(unsigned char**)malloc(sizeof(unsigned char*)*Filas);
        for(i=0; i<Filas; i++)
        {
            Redu[i]=(unsigned char*)malloc(sizeof(unsigned char)*Columnas);
        }
        
        for(i=0; i<Filas; i++)
        {
            for(j=0; j<Columnas; j++)
            {
                fread(&Redu[i][j], sizeof(unsigned char), 1, Archivo);
            }
        }
        Cols=Columnas/2;
        Fils=Filas/2;
        Redu2=(unsigned char**)malloc(sizeof(unsigned char*)*Fils); //Asigna una nueva memoria con el tamanio a la mitad
        for(i=0; i<Fils; i++)
        {
            Redu2[i]=(unsigned char*)malloc(sizeof(unsigned char)*Cols);
        }
        
        for(i=0; i<Filas; i++) //Copia pixeles mientras reduce la imagen
        {
            for(j=0; j<Columnas; j++)
            {
                if(i==0 && j==0)
                    Redu2[i][j]=Redu[i][j];
                if(i==0 && j%2==0)
                    Redu2[i][j/2]=Redu[i][j];
                if(i%2==0 && j==0)
                    Redu2[i/2][j]=Redu[i][j];
                if(i%2==0 && j%2==0)
                    Redu2[i/2][j/2]=Redu[i][j];
            }
        }
        fclose(Archivo);
    }
    
    printf("Gracias.\n");
    printf("\n");
    fpurge(stdin);
    printf("Dame el nombre del archivo en el que deseas guardar la imagen modificada (sin extension): \n");
    gets(NombreArchivo);
    strcat(NombreArchivo, ".pgm");
    Archivos=fopen(NombreArchivo, "wt");
    fprintf(Archivos, "P5 %d %d 255 ", Cols, Fils); //Copia el encabezado ya modificado al nuevo archivo
    for(i=0; i<Fils; i++)
    {
        for(j=0; j<Cols; j++)
        {
            fprintf(Archivos, "%c", Redu2[i][j]); //Copia al archivo el arreglo ya modificado
        }
    }
    fclose(Archivos);
    printf("El archivo se guardo de forma exitosa.\n");
    printf("\nPara ver la imagen modificada, escribe lo siguiente en la terminal:\n\t\temacs NombreDelArchivo.pgm\n");
    for(i=0; i<Filas; i++) //Borra los malloc ya creados
        free(Redu[i]);
    free(Redu);
    for(i=0; i<Fils; i++)
        free(Redu2[i]);
    free(Redu2);
}

void Ampliar()
{
    unsigned char **Ampli, **Ampli2;
    FILE *Archivo, *Archivos;
    char NombreArchivo[20], Encabezado[16];
    int Filas=0, Columnas=0, i, j, Fils=0, Cols=0;
    fpurge(stdin);
    printf("Dame el nombre del archivo con extension (.pgm) que contiene la imagen a modificar: \n");
    gets(NombreArchivo);
    Archivo=fopen(NombreArchivo, "rb");
    if(Archivo==NULL)
    {
        printf("ERROR. El archivo no existe.\n");
        exit(0);
    }
    else
    {
        
        fread(&Encabezado, sizeof(char), 15, Archivo); //Lee el encabezado de la imagen
        sscanf(Encabezado, "P5 %d %d 255 ", &Columnas, &Filas); //Lee el tamanio de la imagen
        Ampli=(unsigned char**)malloc(sizeof(unsigned char*)*Filas);
        for(i=0; i<Filas; i++)
        {
            Ampli[i]=(unsigned char*)malloc(sizeof(unsigned char)*Columnas);
        }
        
        for(i=0; i<Filas; i++)
        {
            for(j=0; j<Columnas; j++)
            {
                fread(&Ampli[i][j], sizeof(unsigned char), 1, Archivo);
            }
        }
        Cols=Columnas*2;
        Fils=Filas*2;
        Ampli2=(unsigned char**)malloc(sizeof(unsigned char*)*Fils); //Asigna el doble de memoria para la imagen ampliada
        for(i=0; i<Fils; i++)
        {
            Ampli2[i]=(unsigned char*)malloc(sizeof(unsigned char)*Cols);
        }
        
        for(i=0; i<Filas; i++) //Copia pixeles mientras amplia la imagen
        {
            for(j=0; j<Columnas; j++)
            {
                Ampli2[i*2][j*2]=Ampli[i][j];
                Ampli2[i*2][j*2+1]=Ampli[i][j];
                Ampli2[i*2+1][j*2]=Ampli[i][j];
                Ampli2[i*2+1][j*2+1]=Ampli[i][j];
            }
            j=0;
        }
        fclose(Archivo);
    }
    printf("Gracias.\n");
    printf("\n");
    fpurge(stdin);
    printf("Dame el nombre del archivo en el que deseas guardar la imagen modificada: \n");
    gets(NombreArchivo);
    strcat(NombreArchivo, ".pgm");
    Archivos=fopen(NombreArchivo, "wt");
    fprintf(Archivos, "P5 %d %d 255 ", Cols, Fils); //Copia al archivo nuevo el encabezado ya modificado
    for(i=0; i<Fils; i++)
    {
        for(j=0; j<Cols; j++)
        {
            fprintf(Archivos, "%c", Ampli2[i][j]); //Copia la archivo nuevo el arreglo con la imagen ya modificada
        }
    }
    printf("El archivo se guardo de forma exitosa.\n");
    printf("\nPara ver la imagen modificada, escribe lo siguiente en la terminal:\n\t\temacs NombreDelArchivo.pgm\n");
    fclose(Archivos);
    for(i=0; i<Filas; i++) //Borra los malloc creados
        free(Ampli[i]);
    free(Ampli);
    for(i=0; i<Fils; i++)
        free(Ampli2[i]);
    free(Ampli2);
}

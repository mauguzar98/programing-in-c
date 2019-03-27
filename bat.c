#include<gtk/gtk.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<stdio.h>

typedef struct Def_Buttons
{
    GtkWidget *mainw, *namew, *optionw, *shipw, *reanudarw;
    GtkLabel *label;
    GtkWidget *buttonsus[10][10];
    GtkWidget *buttonscomp[10][10];
    int pos; //poner barcos
    int x, y, nx, ny; //jugar comp
    int pego, horizontal, vertical,arribaus, abajous, derus, izqus, arribacomp, abajocomp, dercomp, izqcomp;
    int hundido, cuantoscomp, cuantosus, ganador;
    int tx, ty; //jugar us
    int turno; //1 usuario, 2 comp
    int puestos; //barcos puestos para cambiar barco cuando pone el barco
    int ciclo;
    char NombreUs[20];
    char comp[10][10], us[10][10], orgcomp[10][10], orgus[10][10];
    GtkWidget *entrybox;
    struct Def_Buttons *Inicio;
    struct Def_Buttons *ap; //lo necesitamos para reanudar, cuando hace lo de siguiente
    struct Def_Buttons *sig;
}tipoboton;

/*****************VISTA/CONTROLADOR***************/
void Destroy(GtkWidget *window, gpointer data);
GtkWidget *AddButton(GtkWidget *theBox, const gchar *buttonText, gpointer CallBackFunction, tipoboton *EntryBox);
void PopWindow(GtkWidget *button, gpointer data);
void PopWindow1(GtkWidget *button, gpointer data);
void Destroy1(GtkWidget *PopWindows, gpointer data);
void VentanaOpciones(tipoboton *datos);
void VentanaNombres(GtkWidget *PopWindows, gpointer data);
void VentanaBarcos(GtkWidget *PopWindows, gpointer data);
void Posicion(GtkToggleButton *button, gpointer data);
void VentanaJuega(GtkWidget *button, gpointer data);
void VentanaReanudar(tipoboton *datos);
/*************************MODELO********************/
void PonBarcoUs(GtkWidget *button, gpointer data);
void PonBarcosComp(tipoboton *estructura);
void Jugar(GtkWidget *button, gpointer data);
void JugarComp(tipoboton *datos);
void JugarUs(tipoboton *datos);
void CompruebaNombre(GtkWidget *button, gpointer data);
void longitudus(tipoboton *datos);
void Hundirus(tipoboton *datos);
void longitudcomp(tipoboton *datos);
void Hundircomp(tipoboton *datos);
void Inserta(tipoboton *datos);
void LeerArchivo(GtkWidget *button, gpointer data);
void GuardarPartida(GtkWidget *button, gpointer data);
void Siguiente(GtkWidget *button, gpointer data);

gint main(gint argc, gchar *argv[])
{
    GtkWidget *button;
    tipoboton estructura;
    int i, j;

    gtk_init(&argc,&argv);

    for(i=0; i<10; i++)
    {
        for(j=0; j<10; j++)
        {
            estructura.us[i][j]='*';
            estructura.comp[i][j]='*';
            estructura.orgcomp[i][j]='*';
            estructura.orgus[i][j]='*';
        }
    }

    estructura.x=20;
    estructura.y=20;
    estructura.nx=-1;
    estructura.ny=-1;
    estructura.pego=0;
    estructura.vertical=0;
    estructura.horizontal=0;
    estructura.derus=0;
    estructura.izqus=0;
    estructura.arribaus=0;
    estructura.abajous=0;
    estructura.dercomp=0;
    estructura.izqcomp=0;
    estructura.abajocomp=0;
    estructura.arribacomp=0;
    estructura.hundido=0;
    estructura.ganador=0;
    estructura.cuantoscomp=0;
    estructura.cuantosus=0;
    estructura.ciclo=0;
    estructura.Inicio=NULL;
    estructura.ap=NULL;


    VentanaOpciones(&estructura);
    gtk_main();
}

/*
 *Esta funcion despliega una ventana con la opcion de
 *iniciar un nuevo juego o reanudar una partida.
 *@author: Pamela Ortega
 */
void VentanaOpciones(tipoboton *datos)
{
    GtkWidget *nuevo_juego, *resumen;
    GtkWidget *vbox;
    datos->optionw= gtk_window_new(GTK_WINDOW_TOPLEVEL);
    vbox=gtk_vbox_new(TRUE, 0);

    gtk_container_set_border_width(GTK_CONTAINER(datos->optionw), 50);
    gtk_window_set_resizable(GTK_WINDOW(datos->optionw), FALSE);
    gtk_window_set_title(GTK_WINDOW(datos->optionw), "Batalla Naval");
    gtk_signal_connect(GTK_OBJECT(datos->optionw),"destroy", GTK_SIGNAL_FUNC(Destroy), datos);
    nuevo_juego = gtk_button_new_with_label("NUEVO JUEGO");
    resumen = gtk_button_new_with_label("REANUDAR PARTIDA");

    g_signal_connect(nuevo_juego, "clicked", G_CALLBACK(VentanaNombres), datos);
    g_signal_connect(resumen, "clicked", G_CALLBACK(LeerArchivo), datos);

    gtk_widget_set_usize(nuevo_juego, 400, 70);

    gtk_box_pack_start(GTK_BOX(vbox), nuevo_juego, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(vbox), resumen, TRUE, TRUE, 10);

    gtk_container_add(GTK_CONTAINER(datos->optionw), vbox);
    gtk_widget_show_all(datos->optionw);

}

/*
 *Esta funcion se ejecuta cuando el usuario decide que
 *quiere iniciar un nuevo juego. Despliega una ventaja
 *preguntando el nombre del jugador.
 *@author: Pamela Ortega
 *@param: button, la direccion de memoria del boton que lo llamo
 *@param: data, apuntador a la esctructura con todos los parametros
 */
void VentanaNombres(GtkWidget *button, gpointer data)
{
    GtkWidget *accept, *entry;
    GtkWidget *vbox, *hbox;
    tipoboton *datos=(tipoboton *)data;

    gtk_widget_hide_all(datos->optionw);
    datos->namew= gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(datos->namew), "Batalla Naval");
    gtk_window_set_default_size(GTK_WINDOW(datos->namew),200,100);
    accept = gtk_button_new_with_label("Continuar");

    vbox = gtk_vbox_new(TRUE, 0);
    gtk_container_add(GTK_CONTAINER(datos->namew), vbox);

    gtk_signal_connect(GTK_OBJECT(datos->namew),"destroy", GTK_SIGNAL_FUNC(Destroy), datos);
    hbox = gtk_hbox_new(TRUE, 0);
    entry= gtk_entry_new();
    datos->entrybox=entry;

    gtk_container_add(GTK_CONTAINER(hbox), gtk_label_new("Nombre: "));
    gtk_container_add(GTK_CONTAINER(hbox), entry);
    gtk_container_add(GTK_CONTAINER(vbox), hbox);
    gtk_container_add(GTK_CONTAINER(vbox), accept);

    g_signal_connect(accept, "clicked", G_CALLBACK(CompruebaNombre), datos);

    gtk_widget_show_all(datos->namew);
}

/*
 *Esta funcion valida que el usuario haya ingresado su nombre
 *y si no despliega una ventana de error.
 *@author: Pamela Ortega
 *@param: direccion de memoria del boton que la llamo
 *@param: data, estructura con todos los parametros
 */
void CompruebaNombre(GtkWidget *button, gpointer data)
{
    GtkWidget *dialog;
    const gchar *text;
    tipoboton *datos=(tipoboton *)data;

    text=gtk_entry_get_text(GTK_ENTRY(datos->entrybox)); //entrybox con el nombre
    strcpy(datos->NombreUs, text);

    if(datos->NombreUs[0]=='\0')
    {
        dialog=gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_NONE, "Falta nombre");
        gtk_widget_show(dialog);
        gtk_widget_show_all(datos->namew);
    }
    else
    {
        gtk_widget_hide_all(datos->namew);
        VentanaBarcos(button, datos);
    }
}

/*
 *Esta funcion despliega una ventana con el tablero del
 *usuario para que este acomode sus barcos. Le da la opcion de
 *acomodarlos en horizontal o vertical.
 *@author: Pamela Ortega
 *@param: button, direccion de memoria del boton que la llamo
 *@param: data, estructura con los todos los parametros
 */
void VentanaBarcos(GtkWidget *button, gpointer data) //el usuario pone sus barcos
{
    GtkWidget *Vbox1, *hbox1, *label, *rb1, *rb2, *radio, *jugar, *button2;
    int i,j;
    const gchar *text;
    GtkWidget *label1, *label2;
    tipoboton *datos=(tipoboton *)data;

    PonBarcosComp(datos);
    datos->pos=1;
    datos->puestos=0;

    datos->shipw=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(datos->shipw), 400,500);
    gtk_container_border_width(GTK_CONTAINER(datos->shipw), 5);

    gtk_signal_connect(GTK_OBJECT(datos->shipw),"destroy", GTK_SIGNAL_FUNC(Destroy), datos);

    Vbox1=gtk_vbox_new(TRUE, 0);

    label1=gtk_label_new("Elige si quieres tu barco de forma horizontal o vertical.\nSi eliges vertical, el barco se colocara hacia abajo.\nSi eliges horizontal, el barco se colocara hacia la derecha.");
    gtk_box_pack_start(GTK_BOX(Vbox1), label1, TRUE, TRUE, 0);

    radio=gtk_hbox_new(TRUE, 0);
    rb1=gtk_radio_button_new_with_label(NULL, "Horizontal");
    gtk_box_pack_start(GTK_BOX(radio), rb1, FALSE, FALSE, 0);
    gtk_signal_connect(GTK_OBJECT(rb1), "toggled", GTK_SIGNAL_FUNC(Posicion), datos);

    rb2=gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(rb1)), "Vertical");
    gtk_box_pack_start(GTK_BOX(radio), rb2, FALSE, FALSE, 0);
    gtk_signal_connect(GTK_OBJECT(rb2), "toggled", GTK_SIGNAL_FUNC(Posicion), datos);

    label2=gtk_label_new("Inserta el porta aviones (5 cuadros)");
    datos->label=(GtkLabel *)label2;
    label1=gtk_label_new(datos->NombreUs);
    gtk_box_pack_start(GTK_BOX(Vbox1), radio, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(Vbox1), label2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(Vbox1), label1, TRUE, TRUE, 0);
    for(i=0; i<10; i++)
    {
        hbox1=gtk_hbox_new(TRUE, 0);
        for(j=0; j<10; j++)
        {
            button2=AddButton(hbox1, " ", PonBarcoUs, datos);
            datos->buttonsus[i][j]=button2;
        }
        gtk_box_pack_start(GTK_BOX(Vbox1), hbox1, TRUE, TRUE, 0);
    }
    jugar=gtk_button_new_with_label("Jugar");
    gtk_box_pack_start(GTK_BOX(Vbox1), jugar, TRUE, TRUE, 10);
    g_signal_connect(jugar, "clicked", G_CALLBACK(VentanaJuega), datos);
    gtk_window_set_title( GTK_WINDOW(datos->shipw), "BATALLA NAVAL" );

    gtk_container_add(GTK_CONTAINER(datos->shipw), Vbox1);
    gtk_widget_show_all(datos->shipw);
}

/*
 *Esta funcion despliega la ventana principal.
 *Esta ventana es la que permite al usuario jugar en el tablero contrario.
 *La ventana muestra los tableros del usuario y la computadora, el menu de
 *opciones, y un boton de siguiente que se habilita cuando el usuario
 *decide reanudar un juego.
 *@author: Pamela Ortega
 *@param: button, el boton que la llamo.
 *@param: data, la estructura con los parametros
 */
void VentanaJuega(GtkWidget *button, gpointer data)
{
    GdkColor azul, verde, rojo;
    GtkWidget *button3;
    GtkWidget *hbox1, *hbox2, *Vbox1, *Vbox2, *Hbox1;
    GtkWidget *menu, *root_menu, *menu_bar, *op1, *op2;
    GtkWidget *vbox1, *label;
    const gchar *text, *text2;
    int i, j;
    tipoboton *datos=(tipoboton *)data;
    gdk_color_parse("#088A08", &verde);
    gdk_color_parse("#0174DF", &azul);
    gdk_color_parse("#DF0101", &rojo);
    text2=gtk_button_get_label(GTK_BUTTON(button));
    if(datos->puestos==5)
    {
        if(strcmp(text2, "Jugar")==0)
            gtk_widget_hide_all(datos->shipw);
        datos->mainw=gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_default_size(GTK_WINDOW(datos->mainw),600,400);
        gtk_container_border_width(GTK_CONTAINER(datos->mainw), 5);

        gtk_signal_connect(GTK_OBJECT(datos->mainw),"destroy", GTK_SIGNAL_FUNC(Destroy), datos);

        //BARRA DE MENU
        menu_bar=gtk_menu_bar_new();

        //sub2
        root_menu=gtk_menu_item_new_with_label("Guardar partida");
        gtk_menu_bar_append(GTK_MENU_BAR(menu_bar), root_menu);
        gtk_signal_connect(GTK_OBJECT(root_menu), "activate",GTK_SIGNAL_FUNC(GuardarPartida), datos);

        //sub3
        menu=gtk_menu_new();
        op1=gtk_menu_item_new_with_label("Ayuda");
        op2=gtk_menu_item_new_with_label("Desarrolladores");
        gtk_menu_append(GTK_MENU(menu), op1);
        gtk_menu_append(GTK_MENU(menu), op2);

        gtk_signal_connect(GTK_OBJECT(op1), "activate",GTK_SIGNAL_FUNC(PopWindow1), NULL);
        gtk_signal_connect(GTK_OBJECT(op2), "activate",GTK_SIGNAL_FUNC(PopWindow), NULL);
        root_menu=gtk_menu_item_new_with_label("Acerca de");

        gtk_menu_item_set_submenu(GTK_MENU_ITEM(root_menu), menu);

        gtk_menu_bar_append(GTK_MENU_BAR(menu_bar), root_menu);

        //sub4
        root_menu=gtk_menu_item_new_with_label("Terminar partida");
        gtk_signal_connect(GTK_OBJECT(root_menu), "activate",GTK_SIGNAL_FUNC(Destroy), datos);
        gtk_menu_bar_append(GTK_MENU_BAR(menu_bar), root_menu);


        Vbox1=gtk_vbox_new(TRUE, 0);
        label=gtk_label_new(datos->NombreUs);
        gtk_box_pack_start(GTK_BOX(Vbox1), label, TRUE, TRUE, 0);

        for(i=0; i<10; i++)
        {
            hbox1=gtk_hbox_new(TRUE, 0);
            for(j=0; j<10; j++)
            {
                button3=gtk_button_new();
                datos->buttonsus[i][j]=button3;
                if(datos->us[i][j]=='1')
                    gtk_widget_modify_bg(button3, GTK_STATE_NORMAL, &verde); ////////
                if(datos->us[i][j]=='h')
                    gtk_widget_modify_bg(button3, GTK_STATE_NORMAL, &rojo);
                if(datos->us[i][j]=='a')
                    gtk_widget_modify_bg(button3, GTK_STATE_NORMAL, &azul);
                gtk_box_pack_start(GTK_BOX(hbox1), button3, TRUE, TRUE, 0);
            }
            gtk_box_pack_start(GTK_BOX(Vbox1), hbox1, TRUE, TRUE, 0);
        }
        Vbox2=gtk_vbox_new(TRUE, 0);
        label=gtk_label_new("COMPUTADORA");
        gtk_box_pack_start(GTK_BOX(Vbox2), label, TRUE, TRUE, 0);
        for(i=0; i<10; i++) //arreglo de botones de la compu
        {
            hbox2=gtk_hbox_new(TRUE, 0);
            for(j=0; j<10; j++)
            {
                datos->buttonscomp[i][j]=AddButton(hbox2, " ", Jugar, datos);
                if(datos->comp[i][j]=='a')
                    gtk_widget_modify_bg(datos->buttonscomp[i][j], GTK_STATE_NORMAL, &azul);
                if(datos->comp[i][j]=='h')
                    gtk_widget_modify_bg(datos->buttonscomp[i][j], GTK_STATE_NORMAL, &rojo);
            }
            gtk_box_pack_start(GTK_BOX(Vbox2), hbox2, TRUE, TRUE, 0);
        }


        Hbox1=gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(Hbox1), Vbox1, TRUE, TRUE, 20);
        gtk_box_pack_start(GTK_BOX(Hbox1), Vbox2, TRUE, TRUE, 20);

        vbox1=gtk_vbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox1), menu_bar, FALSE, FALSE, 10);
        gtk_box_pack_start(GTK_BOX(vbox1), Hbox1, FALSE, FALSE, 0);

        gtk_window_set_title( GTK_WINDOW(datos->mainw), "BATALLA NAVAL" );

        gtk_container_add(GTK_CONTAINER(datos->mainw), vbox1);
        gtk_widget_show_all(datos->mainw);
    }
}

/*
 *Esta funcion es ejecutada cuando el usuario clickea un boton en el
 *tablero de la computadora donde quiere tirar. Esta llama a JugarUs y
 *JugarComp, tambien genera las coordenadas del tiro inteligente de la
 *computadora.
 *@author: Dalia Braverman, Juan Jose Aguero, Pamela Ortega
 *@param: button, direccion de memoria del boton que la llamo
 *@param: data, estructura con todos los parametros.
 */
void Jugar(GtkWidget *button, gpointer data)
{
    tipoboton *datos=(tipoboton*)data;
    int i, j;
    GtkWidget *window, *label; //hundio la compu
    GtkWidget *window1; //hundido us
    GtkWidget *window2; //gano us
    GtkWidget *window3; //repitio el tiro
    GtkWidget *window4; //gano comp

    if(datos->ganador==0)
    {
        datos->turno=1;
        for(i=0; i<10; i++)
        {
            for(j=0; j<10; j++)
            {
                if(button==datos->buttonscomp[i][j])
                {
                    datos->tx=i;
                    datos->ty=j;
                }
            }
        }
        JugarUs(datos);
        if(datos->ciclo==0)
        {
            if(datos->comp[datos->tx][datos->ty]=='h')
            {
                longitudus(datos);
                Hundirus(datos);
            }
            if(datos->hundido==1)
            {
                datos->cuantosus++;
                window1=gtk_window_new(GTK_WINDOW_TOPLEVEL);
                gtk_container_border_width(GTK_CONTAINER(window1), 5);
                gtk_window_set_title(GTK_WINDOW(window1), "FELICIDADES");
                gtk_window_set_default_size(GTK_WINDOW(window1), 210, 100);
                label=gtk_label_new("¡FELICIDADES!\nHundiste un barco\n\tSigue asi");
                gtk_signal_connect(GTK_OBJECT(window1), "destroy", GTK_SIGNAL_FUNC(Destroy1), NULL);
                gtk_container_add(GTK_CONTAINER(window1), label);
                gtk_widget_show_all(window1);
            }
            Inserta(datos);
            datos->turno=2;
            datos->hundido=0;
            /* for(h=0; h<80000000; h++)
             {
             //vamos a poner un label que sea turno jugador y turno de la compu
             }*/
            if((datos->x==20)&&(datos->y==20))
            {
                do
                {
                    datos->x=0+rand()%((9+1)+0);
                    datos->y=0+rand()%((9+1)+0);
                }while ((datos->us[datos->x][datos->y]=='h')||(datos->us[datos->x][datos->y]=='a'));
                JugarComp(datos);
                Inserta(datos);
            }
            else
            {
                JugarComp(datos);
                Inserta(datos);
            }
            if((datos->pego!=0)||(datos->vertical!=0)||(datos->horizontal!=0))
            {
                if((datos->nx==-1)&&(datos->ny==-1))
                {
                    datos->nx=datos->x+1;
                    datos->ny=datos->y;
                }
                else
                    Hundircomp(datos);
                if((datos->vertical==1))
                {
                    datos->nx=datos->x-1;
                    datos->ny=datos->y;
                }
                else
                {
                    if(datos->vertical==2)
                    {
                        datos->nx=datos->nx+1;
                        datos->ny=datos->y;
                    }
                    else
                    {
                        if(datos->vertical==3)
                        {
                            datos->nx=datos->nx-1;
                            datos->ny=datos->y;
                        }
                        else
                        {
                            if(datos->horizontal==1)
                            {
                                datos->nx=datos->x;
                                datos->ny=datos->y-1;
                            }
                            else
                            {
                                if(datos->horizontal==2)
                                {
                                    datos->nx=datos->x;
                                    datos->ny=datos->ny+1;
                                }
                                else
                                {
                                    if(datos->horizontal==3)
                                    {
                                        datos->nx=datos->x;
                                        datos->ny=datos->ny-1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if((datos->pego==0)&&(datos->vertical==0)&&(datos->horizontal==0))
            {
                datos->x=20;
                datos->y=20;
                datos->nx=-1;
                datos->ny=-1;
            }
            datos->turno=1;
            if(datos->hundido==1)
            {
                datos->x=20;
                datos->y=20;
                datos->nx=-1;
                datos->ny=-1;
                datos->vertical=0;
                datos->horizontal=0;
                datos->pego=0;
                datos->cuantoscomp++;
                window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
                gtk_container_border_width(GTK_CONTAINER(window), 5);
                gtk_window_set_title(GTK_WINDOW(window), "LASTIMA");
                gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);
                label=gtk_label_new("\t\t¡UPS!\nTu rival hundio un barco");
                gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(Destroy1), NULL);
                gtk_container_add(GTK_CONTAINER(window), label);
                gtk_widget_show_all(window);
            }
            datos->hundido=0;
            if(datos->cuantoscomp==5)
                datos->ganador=1;
            if(datos->cuantosus==5)
                datos->ganador=2;
            if (datos->ganador!=0)
            {
                if(datos->ganador==1)
                {
                    gtk_widget_hide_all(window);
                    window4=gtk_window_new(GTK_WINDOW_TOPLEVEL);
                    gtk_container_border_width(GTK_CONTAINER(window4), 5);
                    gtk_window_set_title(GTK_WINDOW(window4), "LASTIMA");
                    gtk_window_set_default_size(GTK_WINDOW(window4), 200, 100);
                    label=gtk_label_new("\t¡UY TE GANARON!\nMejor suerte para la proxima");
                    gtk_signal_connect(GTK_OBJECT(window4), "destroy", GTK_SIGNAL_FUNC(Destroy1), NULL);
                    gtk_container_add(GTK_CONTAINER(window4), label);
                    gtk_widget_show_all(window4);
                }
                if(datos->ganador==2)
                {
                    gtk_widget_hide_all(window1);
                    window2=gtk_window_new(GTK_WINDOW_TOPLEVEL);
                    gtk_container_border_width(GTK_CONTAINER(window2), 5);
                    gtk_window_set_title(GTK_WINDOW(window2), "FELICIDADES");
                    gtk_window_set_default_size(GTK_WINDOW(window2), 210, 100);
                    label=gtk_label_new("¡Felicidades!\nGANASTE");
                    gtk_signal_connect(GTK_OBJECT(window2), "destroy", GTK_SIGNAL_FUNC(Destroy1),NULL);
                    gtk_container_add(GTK_CONTAINER(window2), label);
                    gtk_widget_show_all(window2);
                }
            }
        }
        else
        {
            //gtk_widget_hide_all(window1);
            window3=gtk_window_new(GTK_WINDOW_TOPLEVEL);
            gtk_container_border_width(GTK_CONTAINER(window3), 5);
            gtk_window_set_title(GTK_WINDOW(window3), "ERROR");
            gtk_window_set_default_size(GTK_WINDOW(window3), 200, 100);
            label=gtk_label_new("Ahi ya tiraste\nIntenta otra vez");
            gtk_signal_connect(GTK_OBJECT(window3), "destroy", GTK_SIGNAL_FUNC(Destroy1), NULL);
            gtk_container_add(GTK_CONTAINER(window3), label);
            gtk_widget_show_all(window3);
        }
    }
}

/*
 *Esta funcion ejecuta el tiro de la computadora
 *y regresa banderas que ayudan a la funcion Jugar
 *a generar las coordenadas del siguiente tiro inteligente.
 *@author: Dalia Braverman, Juan Jose Aguero
 *@param: datos, estructura con todos los parametros.
 */
void JugarComp(tipoboton *datos)
{
    GdkColor azul;
    GdkColor rojo;
    int tiro=0;
    gdk_color_parse("#0174DF", &azul);
    gdk_color_parse("#DF0101", &rojo);
    if((datos->nx==-1)&&(datos->ny==-1)) //Primer tiro
    {
        if(datos->us[datos->x][datos->y]=='*')
        {
            datos->us[datos->x][datos->y]='a';
            gtk_widget_modify_bg(datos->buttonsus[datos->x][datos->y], GTK_STATE_NORMAL, &azul);
        }
        if(datos->us[datos->x][datos->y]=='1')
        {
            longitudcomp(datos);
            datos->pego=1;
            datos->us[datos->x][datos->y]='h';
            gtk_widget_modify_bg(datos->buttonsus[datos->x][datos->y], GTK_STATE_NORMAL, &rojo);
        }
    }
    else //tiro inteligente
    {
        if(datos->nx>datos->x)//abajo
        {
            if((datos->nx>9)||(datos->us[datos->nx][datos->ny]=='a')) //tiro no valido, lo manda hacia el primero hacia arriba.
            {
                datos->nx=datos->x-1;
                datos->ny=datos->y;
            }
            else
            {
                if(datos->us[datos->nx][datos->ny]=='1')//le pego
                {
                    datos->vertical=2;
                    datos->us[datos->nx][datos->ny]='h';
                    gtk_widget_modify_bg(datos->buttonsus[datos->nx][datos->ny], GTK_STATE_NORMAL, &rojo);
                    tiro=1;
                }
                else
                {
                    if(datos->us[datos->nx][datos->ny]=='*')//fallo
                    {
                        datos->vertical=1;
                        datos->pego=0;
                        datos->us[datos->nx][datos->ny]='a';
                        gtk_widget_modify_bg(datos->buttonsus[datos->nx][datos->ny], GTK_STATE_NORMAL, &azul);
                        tiro=1;
                    }
                }
            }
        }
        if(tiro==0)
        {
            if(datos->nx<datos->x)//arriba
            {
                if ((datos->nx<0)||(datos->us[datos->nx][datos->ny]=='a')) //no valido, lo manda al primero hacia la derecha.
                {
                    datos->nx=datos->x;
                    datos->ny=datos->y+1;
                    datos->vertical=4;
                }
                else
                {
                    if(datos->us[datos->nx][datos->ny]=='1')
                    {
                        datos->vertical=3;
                        datos->us[datos->nx][datos->ny]='h';
                        gtk_widget_modify_bg(datos->buttonsus[datos->nx][datos->ny], GTK_STATE_NORMAL, &rojo);
                        datos->pego=1;
                        tiro=1;
                    }
                    else
                    {
                        if(datos->us[datos->nx][datos->ny]=='*')
                        {
                            datos->us[datos->nx][datos->ny]='a';
                            gtk_widget_modify_bg(datos->buttonsus[datos->nx][datos->ny], GTK_STATE_NORMAL, &azul);
                            datos->vertical=4;
                            datos->nx=datos->x;
                            datos->ny=datos->y+1;
                            tiro=1;
                        }
                    }
                }
            }
            if(tiro==0)
            {
                if(datos->ny>datos->y)//derecha
                {
                    if ((datos->ny>9)||(datos->us[datos->nx][datos->ny]=='a')) //no valido, lo manda al primero hacia la izquierda.
                    {
                        datos->ny=datos->y-1;
                        datos->nx=datos->x;
                    }
                    else
                    {
                        if(datos->us[datos->nx][datos->ny]=='1')
                        {
                            datos->us[datos->nx][datos->ny]='h';
                            gtk_widget_modify_bg(datos->buttonsus[datos->nx][datos->ny], GTK_STATE_NORMAL, &rojo);
                            datos->horizontal=2;
                            datos->pego=1;
                            tiro=1;
                        }
                        else
                        {
                            if(datos->us[datos->nx][datos->ny]=='*')
                            {
                                datos->horizontal=1;
                                datos->pego=0;
                                datos->us[datos->nx][datos->ny]='a';
                                gtk_widget_modify_bg(datos->buttonsus[datos->nx][datos->ny], GTK_STATE_NORMAL, &azul);
                                tiro=1;
                            }
                        }
                    }
                }
                if(tiro==0)
                {
                    if(datos->ny<datos->y)//izquierda
                    {
                        if(datos->us[datos->nx][datos->ny]=='1')
                        {
                            datos->us[datos->nx][datos->ny]='h';
                            gtk_widget_modify_bg(datos->buttonsus[datos->nx][datos->ny], GTK_STATE_NORMAL, &rojo);
                            datos->pego=1;
                            datos->horizontal=3;
                        }
                        else
                        {
                            if(datos->us[datos->nx][datos->ny]=='*')
                            {
                                datos->horizontal=4;
                                datos->us[datos->nx][datos->ny]='a';
                                gtk_widget_modify_bg(datos->buttonsus[datos->nx][datos->ny], GTK_STATE_NORMAL, &azul);
                                datos->pego=0;
                            }
                        }
                    }
                }
            }
        }
    }
}

/*
 *Esta funcion ejecuta el tiro del jugador. Valida que
 *el tiro no se repita, modifica el arreglo segun el tiro y
 *cambia de color el boton presionado.
 *@author: Dalia Braverman
 *@param: estructura, estructura con todos los parametros
 */
void JugarUs(tipoboton *estructura)
{
    GdkColor azul;
    GdkColor rojo;
    gdk_color_parse("#0174DF", &azul);
    gdk_color_parse("#DF0101", &rojo);
    estructura->ciclo=0;
    if((estructura->comp[estructura->tx][estructura->ty]=='a')||(estructura->comp[estructura->tx][estructura->ty]=='h'))
    {
        estructura->ciclo=1;
        return;
    }
    if(estructura->comp[estructura->tx][estructura->ty]=='*')
    {
        estructura->comp[estructura->tx][estructura->ty]='a';
        gtk_widget_modify_bg(estructura->buttonscomp[estructura->tx][estructura->ty], GTK_STATE_NORMAL, &azul);
    }
    if(estructura->comp[estructura->tx][estructura->ty]=='1')
    {
        estructura->comp[estructura->tx][estructura->ty]='h';
        gtk_widget_modify_bg(estructura->buttonscomp[estructura->tx][estructura->ty], GTK_STATE_NORMAL, &rojo);
    }
}

/*
 *Esta funcion cuenta la cantidad de cuadros que ocupa el
 *barco que fue golpeado.
 *@author: Juan Jose Aguero
 *@param: datos, estructura con todos los parametros
 */
void longitudus(tipoboton *datos)
{
    int i=1;
    datos->arribaus=0;
    datos->abajous=0;
    datos->izqus=0;
    datos->derus=0;

    while ((datos->tx-i>=0)&&((datos->comp[datos->tx-i][datos->ty]=='h')||(datos->comp[datos->tx-i][datos->ty]=='1')))
    {
        datos->arribaus++;
        i++;
    }
    i=1;
    while ((datos->tx+i<10)&&((datos->comp[datos->tx+i][datos->ty]=='h')||(datos->comp[datos->tx+i][datos->ty]=='1')))
    {
        datos->abajous++;
        i++;
    }
    i=1;
    while ((datos->ty-i>=0)&&((datos->comp[datos->tx][datos->ty-i]=='h')||(datos->comp[datos->tx][datos->ty-i]=='1')))
    {

        datos->izqus++;
        i++;

    }
    i=1;
    while ((datos->ty+i<10)&&((datos->comp[datos->tx][datos->ty+i]=='h')||(datos->comp[datos->tx][datos->ty+i]=='1')))
    {
        datos->derus++;
        i++;
    }
}

/*
 *Esta funcion valida si el usuario ya hundio el barco al que le esta dando.
 *@author: Juan Jose Aguero
 *@param: datos, estructura con todos los parametros
 */
void Hundirus(tipoboton *datos)
{
    int i=0;
    if((datos->derus!=0)||(datos->izqus!=0))
    {
        if(datos->izqus!=0)
        {
            for(i=datos->ty; i>((datos->ty)-(datos->izqus)); i--)
            {
                datos->hundido=0;
                if((i<10)&&(i>0))
                {
                    if((datos->comp[datos->tx][i-1]=='h'))
                        datos->hundido=1;
                    else
                        return;
                }
            }
        }
        if(datos->derus!=0)
        {
            for(i=datos->ty; i<((datos->ty)+(datos->derus)); i++)
            {
                datos->hundido=0;
                if((i<10)&&(i>=0))
                {
                    if((datos->comp[datos->tx][i+1]=='h'))
                        datos->hundido=1;
                    else
                        return;
                }
            }
        }
    }
    else
    {
        if(datos->abajous!=0)
        {
            for(i=datos->tx; i<((datos->tx)+(datos->abajous)); i++)
            {
                datos->hundido=0;
                if((i<10)&&(i>=0))
                {
                    if((datos->comp[i+1][datos->ty]=='h'))
                        datos->hundido=1;
                    else
                        return;
                }
            }
        }
        if(datos->arribaus!=0)
        {
            for(i=datos->tx; i>((datos->tx)-(datos->arribaus)); i--)
            {
                datos->hundido=0;
                if((i<10)&&(i>0))
                {
                    if((datos->comp[i-1][datos->ty]=='h'))
                        datos->hundido=1;
                    else
                        return;
                }
            }
        }
    }
}

/*
 *Esta funcion cuenta la cantidad de cuadros que ocupa
 *el barco que golpeo la computadora.
 *@author: Juan Jose Aguero
 *@param: datos, estructura con los parametros
 */
void longitudcomp(tipoboton *datos)
{
    int i=1;
    datos->arribacomp=0;
    datos->abajocomp=0;
    datos->izqcomp=0;
    datos->dercomp=0;

    while ((datos->x-i>=0)&&(datos->us[datos->x-i][datos->y]=='1'))
    {
        datos->arribacomp++;
        i++;
    }
    i=1;
    while ((datos->x+i<10)&&(datos->us[datos->x+i][datos->y]=='1'))
    {
        datos->abajocomp++;
        i++;
    }
    i=1;
    while ((datos->y-i>=0)&&(datos->us[datos->x][datos->y-i]=='1'))
    {
        datos->izqcomp++;
        i++;
    }
    i=1;
    while ((datos->y+i<10)&&(datos->us[datos->x][datos->y+i]=='1'))
    {
        datos->dercomp++;
        i++;
    }
}

/*
 *Esta funcion valida si el barco al que la computadora esta tirando
 *se hundio.
 *@author: Juan Jose Aguero y Dalia Braverman
 *@param: datos, estructura con todos los parametros
 */
void Hundircomp(tipoboton *datos)
{
    int i=0;
    if((datos->dercomp!=0)||(datos->izqcomp!=0))
    {
        if(datos->izqcomp!=0)
        {
            for(i=datos->y; i>((datos->y)-(datos->izqcomp)); i--)
            {
                datos->hundido=0;
                if((i<10)&&(i>0))
                {
                    if((datos->us[datos->x][i-1]=='h'))
                        datos->hundido=1;
                    else
                        return;
                }
            }
        }
        if(datos->dercomp!=0)
        {
            for(i=datos->y; i<((datos->y)+(datos->dercomp)); i++)
            {
                datos->hundido=0;
                if((i<10)&&(i>=0))
                {
                    if((datos->us[datos->x][i+1]=='h'))
                        datos->hundido=1;
                    else
                        return;
                }
            }
        }
    }
    else
    {
        if(datos->abajocomp!=0)
        {
            for(i=datos->x; i<((datos->x)+(datos->abajocomp)); i++)
            {
                datos->hundido=0;
                if((i<10)&&(i>=0))
                {
                    if((datos->us[i+1][datos->y]=='h'))
                        datos->hundido=1;
                    else
                        return;
                }
            }
        }
        if(datos->arribacomp!=0)
        {
            for(i=datos->x; i>((datos->x)-(datos->arribacomp)); i--)
            {
                datos->hundido=0;
                if((i<10)&&(i>0))
                {
                    if((datos->us[i-1][datos->y]=='h'))
                        datos->hundido=1;
                    else
                        return;
                }
            }
        }
    }
}
/*
 *Esta funcion borra la lista dinamica y finaliza el programa.
 *@author: Pamela Ortega
 *@param: window, ventana que va a cerrar
 *@param: data, estructura con todos los parametros
 *
 */
void Destroy(GtkWidget *window, gpointer data)
{
    tipoboton *temp;
    tipoboton *datos=(tipoboton*)data;
    temp=datos->Inicio;
    while(temp!=NULL)
    {
        datos->Inicio=((datos->Inicio)->sig);
        free(temp);
        temp=datos->Inicio;
    }
    gtk_main_quit();
}

void Destroy1(GtkWidget *PopWindows, gpointer data)
{
    gtk_widget_destroy(PopWindows);
}

GtkWidget *AddButton(GtkWidget *theBox, const gchar *buttonText, gpointer CallBackFunction, tipoboton *EntryBox)
{
    GtkWidget *button;
    button = gtk_button_new_with_label(buttonText);
    gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE,0);
    gtk_signal_connect(GTK_OBJECT(button),"clicked",GTK_SIGNAL_FUNC(CallBackFunction),EntryBox);
    gtk_widget_show(button);
    return button;
}

void PopWindow(GtkWidget *button, gpointer data)
{
    GtkWidget *PopWindows, *label;
    PopWindows=gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_container_border_width(GTK_CONTAINER(PopWindows), 5);
    label=gtk_label_new("Programa desarrollado por:\n\n\tAguero Vargas Juan Jose\n\tBraverman Jaiven Dalia\n\tOrtega De la Vega Pamela Fernanda\n\nMateria:\n\tProgramacion Aplicada y Lab\n\n\tMtro. Jorge Rodriguez\n\n\t\tOtoño 2018");
    gtk_signal_connect(GTK_OBJECT(PopWindows),"destroy", GTK_SIGNAL_FUNC(Destroy1), NULL);
    gtk_window_set_title( GTK_WINDOW(PopWindows), "DESARROLLADORES");
    gtk_container_add(GTK_CONTAINER(PopWindows), label);
    gtk_widget_show_all(PopWindows);
}

void PopWindow1(GtkWidget *button, gpointer data)
{
    GtkWidget *PopWindows, *label;
    PopWindows=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_container_border_width(GTK_CONTAINER(PopWindows), 5);
    gtk_widget_set_size_request(PopWindows, 520, 280);
    gtk_window_set_resizable(GTK_WINDOW(PopWindows), FALSE);
    label=gtk_label_new("\t\t\t\t\t\tInstrucciones:\nEl objetivo principal es hundir la flota del contrario eligiendo campos del \ntablero enemigo estrategicamente para dar con la ubicacion de todos los \nbarcos y bombardearlos hasta hundirlos.Se juega con dos tableros de 10x10.\nCada jugador tiene un tiro por turno en el que tiene que tratar de dar con la \nubicacion de los barcos contrarios, el primer jugador que logre hundir toda la \nflota contraria sera el ganador.Un barco se considerara 'hundido' cuando \ntodos los espacios que ocupa hayan sido bombardeados.\n\nCoordenada roja: el barco ha sido golpeado.\nCoordenada azul: Fallaste.\nCoordenada verde: Ubicacion de tus barcos.\nDeberás tirar en el tablero con titulo 'COMPUTADORA'\n\n\t\t\t\t\t\t\t¡Suerte!\n");
    gtk_signal_connect(GTK_OBJECT(PopWindows),"destroy", GTK_SIGNAL_FUNC(Destroy1), NULL);
    gtk_window_set_title( GTK_WINDOW(PopWindows), "AYUDA" );
    gtk_container_add(GTK_CONTAINER(PopWindows), label);
    gtk_widget_show_all(PopWindows);
}

void Posicion(GtkToggleButton *button, gpointer data)
{
    tipoboton *datos=(tipoboton*)data;
    const char *text;
    char text2[20];
    if(gtk_toggle_button_get_active(button))
    {
        text=gtk_button_get_label(GTK_BUTTON(button));
        strcpy(text2, text);
        if(strcmp(text2, "Vertical")==0)
            datos->pos=0;
        else
            datos->pos=1;
    }
}

void PonBarcoUs(GtkWidget *button, gpointer data) //pone los barcos del us y lo cambia en el arreglo y en el tablero
{
    int i, j, n=0, p=0;
    int barco, bandera;
    int px, py;
    tipoboton *datos=(tipoboton*)data;
    GdkColor color;

    gdk_color_parse("#088A08", &color);

    if(datos->puestos<=4) //valida que solo ponga 5 barcos.
    {
        if(datos->puestos==0)
        {
            barco=5;
            gtk_label_set_text(datos->label, "Inserta el acorazado (4 cuadros)");
        }
        if(datos->puestos==1)
        {
            barco=4;
            gtk_label_set_text(datos->label, "Inserta el destructor (3 cuadros)");
        }
        if((datos->puestos==2)||(datos->puestos==3))
        {
            if(datos->puestos==3)
                gtk_label_set_text(datos->label, "Inserta el bote (2 cuadros)");
            if(datos->puestos==2)
                gtk_label_set_text(datos->label, "Inserta el submarino (3 cuadros)");
            barco=3;
        }
        if(datos->puestos==4)
        {
            barco=2;
            gtk_label_set_text(datos->label, "Tu barcos estan listos para jugar");
        }


        for(i=0; i<10; i++)
        {
            for(j=0; j<10; j++)
            {
                if(button==datos->buttonsus[i][j])
                {
                    px=i;
                    py=j;
                }
            }
        }
        bandera=0;
        if(datos->pos==0)
        {
            for(i=px; i<px+barco; i++)
            {
                if(i>9)
                    bandera=1;
                //Aquí se validan los bordes
                if (px==0) // 1 2 3
                {
                    if (py-1<0)// 1
                    {
                        if(i==px)//primera validación
                        {
                            if (datos->us[i][py+1]!='*')
                                bandera=1;
                        }
                        else
                        {
                            if (i==px+barco-1)//Ultima
                            {
                                if ((datos->us[i][py+1]!='*')||(datos->us[i+1][py]!='*'))
                                    bandera=1;
                            }
                            else
                            {
                                if (datos->us[i][py+1]!='*') //inter
                                    bandera=1;
                            }
                        }
                    }
                    else
                    {
                        if ((py-1>=0)&&(py+1<=9))//2
                        {
                            if(i==px)//primera validación
                            {
                                if ((datos->us[i][py+1]!='*')||(datos->us[i][py-1]!='*'))
                                    bandera=1;
                            }
                            else
                            {
                                if (i==px+barco-1)//Ultima
                                {
                                    if ((datos->us[i][py+1]!='*')||(datos->us[i][py-1]!='*')||(datos->us[i+1][py]!='*'))
                                        bandera=1;
                                }
                                else
                                {
                                    if ((datos->us[i][py+1]!='*')||(datos->us[i][py-1]!='*')) //inter
                                        bandera=1;
                                }

                            }
                        }
                        else
                        {
                            if (py==9)//3
                            {
                                if(i==px)//primera validación
                                {
                                    if (datos->us[i][py-1]!='*')
                                        bandera=1;
                                }
                                else
                                {
                                    if (i==px+barco-1)//Ultima
                                    {
                                        if ((datos->us[i][py-1]!='*')||(datos->us[i+1][py]!='*'))
                                            bandera=1;
                                    }
                                    else
                                    {
                                        if (datos->us[i][py-1]!='*') //inter
                                            bandera=1;
                                    }
                                }
                            }
                        }
                    }
                }
                if (px!=0) //4 5 6
                {
                    if (py==0) //4
                    {
                        if(i==px)//primera validación
                        {
                            if ((datos->us[i][py+1]!='*')||(datos->us[i-1][py]!='*'))
                                bandera=1;
                        }
                        else
                        {
                            if (i==px+barco-1)//Ultima
                            {
                                if(i==9)//queda justo
                                {
                                    if (datos->us[i][py+1]!='*')
                                        bandera=1;
                                }
                                else //no queda justo
                                {
                                    if ((datos->us[i+1][py]!='*')||(datos->us[i][py+1]!='*'))
                                        bandera=1;
                                }
                            }
                            else//inter
                            {
                                if (datos->us[i][py+1]!='*')
                                    bandera=1;
                            }
                        }
                    }
                    else //5 6
                    {
                        if (py==9) //6
                        {
                            if(i==px)//primera validación
                            {
                                if ((datos->us[i][py-1]!='*')||(datos->us[i-1][py]!='*'))
                                    bandera=1;
                            }
                            else
                            {
                                if (i==px+barco-1)//Ultima
                                {
                                    if(i==9)//queda justo
                                    {
                                        if (datos->us[i][py-1]!='*')
                                            bandera=1;
                                    }
                                    else //no queda justo
                                    {
                                        if ((datos->us[i+1][py]!='*')||(datos->us[i][py-1]!='*'))
                                            bandera=1;
                                    }
                                }
                                else//inter
                                {
                                    if (datos->us[i][py-1]!='*')
                                        bandera=1;
                                }
                            }
                        }
                        else //5
                        {
                            if(i==px)//primera validación
                            {
                                if ((datos->us[i][py-1]!='*')||(datos->us[i-1][py]!='*')||(datos->us[i][py+1]!='*'))
                                    bandera=1;
                            }
                            else
                            {
                                if (i==px+barco-1)//Ultima
                                {
                                    if(i==9)//queda justo
                                    {
                                        if ((datos->us[i][py-1]!='*')||(datos->us[i][py+1]!='*'))
                                            bandera=1;
                                    }
                                    else //no queda justo
                                    {
                                        if ((datos->us[i+1][py]!='*')||(datos->us[i][py-1]!='*')||(datos->us[i][py+1]!='*'))
                                            bandera=1;
                                    }
                                }
                                else//inter
                                {
                                    if ((datos->us[i][py-1]!='*')||(datos->us[i][py+1]!='*'))
                                        bandera=1;
                                }
                            }
                        }
                    }
                }
            }
        }
        else //pos 1
        {
            for(i=py; i<py+barco; i++)
            {
                if(i>9)
                    bandera=1;
                if (py==0) //7 8 9
                {
                    if (px==0) //7
                    {
                        if (i==(py+barco-1))// ultima
                        {
                            if ((datos->us[px][i+1]!='*')||(datos->us[px+1][i]!='*'))
                                bandera=1;
                        }
                        else //Primero inter
                        {
                            if (datos->us[px+1][i]!='*')
                                bandera=1;
                        }
                    }
                    else
                    {
                        if (px==9)// 9
                        {
                            if (i==(py+barco-1))// ultima
                            {
                                if ((datos->us[px][i+1]!='*')||(datos->us[px-1][i]!='*'))
                                    bandera=1;
                            }
                            else //Primero inter
                            {
                                if (datos->us[px-1][i]!='*')
                                    bandera=1;
                            }
                        }
                        else //8
                        {
                            if (i==(py+barco-1))// ultima
                            {
                                if ((datos->us[px][i+1]!='*')||(datos->us[px+1][i]!='*')||(datos->us[px-1][i]!='*'))
                                    bandera=1;
                            }
                            else //Primero inter
                            {
                                if ((datos->us[px+1][i]!='*')||(datos->us[px-1][i]!='*'))
                                    bandera=1;
                            }
                        }
                    }
                }
                if (py!=0)//10 11 12
                {
                    if (px==0) //10
                    {
                        if (i==py)//primero
                        {
                            if ((datos->us[px][i-1]!='*')||(datos->us[px+1][i]!='*'))
                                bandera=1;
                        }
                        else
                        {
                            if (i==(py+barco-1)) //ultimo
                            {
                                if (i==9)//justo
                                {
                                    if (datos->us[px+1][i]!='*')
                                        bandera=1;
                                }
                                else//no justo
                                {
                                    if ((datos->us[px+1][i]!='*')||(datos->us[px][i+1]!='*'))
                                        bandera=1;
                                }

                            }
                            else //inter
                            {
                                if (datos->us[px+1][i]!='*')
                                    bandera=1;
                            }
                        }
                    }
                    else
                    {
                        if (px==9) //12
                        {
                            if (i==py)//primero
                            {
                                if ((datos->us[px][i-1]!='*')||(datos->us[px-1][i]!='*'))
                                    bandera=1;
                            }
                            else
                            {
                                if (i==(py+barco-1)) //ultimo
                                {
                                    if (i==9)//justo
                                    {
                                        if (datos->us[px-1][i]!='*')
                                            bandera=1;
                                    }
                                    else//no justo
                                    {
                                        if ((datos->us[px-1][i]!='*')||(datos->us[px][i+1]!='*'))
                                            bandera=1;
                                    }

                                }
                                else //inter
                                {
                                    if (datos->us[px-1][i]!='*')
                                        bandera=1;
                                }
                            }

                        }
                        else //11
                        {
                            if (i==py)//primer
                            {
                                if((datos->us[px][i-1]!='*')||(datos->us[px-1][i]!='*')||(datos->us[px+1][i]!='*'))
                                    bandera=1;
                            }
                            else
                            {
                                if (i==(py+barco)-1)//ultimo
                                {
                                    if (i==9)//justo
                                    {
                                        if((datos->us[px-1][i]!='*')||(datos->us[px+1][i]!='*'))
                                            bandera=1;
                                    }
                                    else //no justo
                                    {
                                        if((datos->us[px][i+1]!='*')||(datos->us[px-1][i]!='*')||(datos->us[px+1][i]!='*'))
                                            bandera=1;
                                    }
                                }
                                else //inter
                                {
                                    if((datos->us[px-1][i]!='*')||(datos->us[px+1][i]!='*'))
                                        bandera=1;
                                }
                            }
                        }
                    }
                }
            }
        }
        if(bandera==0)
        {

            if(datos->pos==0)
            {
                p=px;
                while(n<barco)
                {
                    datos->us[p][py]='1';
                    datos->orgus[p][py]='1';
                    gtk_button_set_label(GTK_BUTTON(datos->buttonsus[p][py]), "B");
                    gtk_widget_set_sensitive(datos->buttonsus[p][py], FALSE);
                    if (px==0) // 1 2 3
                    {
                        if (py-1<0)// 1
                        {
                            if(n==0)
                                gtk_widget_set_sensitive(datos->buttonsus[p][py+1], FALSE);
                            if (n==barco-1)
                            {
                                gtk_widget_set_sensitive(datos->buttonsus[p+1][py], FALSE);
                                gtk_widget_set_sensitive(datos->buttonsus[p][py+1], FALSE);
                            }
                            else
                                gtk_widget_set_sensitive(datos->buttonsus[p][py+1], FALSE);
                        }
                        if ((py-1>=0)&&(py+1<=9))//2
                        {
                            if(n==barco-1)//ultima
                            {
                                gtk_widget_set_sensitive(datos->buttonsus[p+1][py], FALSE);
                                gtk_widget_set_sensitive(datos->buttonsus[p][py+1], FALSE);
                                gtk_widget_set_sensitive(datos->buttonsus[p][py-1], FALSE);
                            }
                            else
                            {
                                gtk_widget_set_sensitive(datos->buttonsus[p][py-1], FALSE);
                                gtk_widget_set_sensitive(datos->buttonsus[p][py+1], FALSE);
                            }
                        }
                        if (py==9)//3
                        {
                            if(n==barco-1)//ultima
                            {
                                gtk_widget_set_sensitive(datos->buttonsus[p+1][py], FALSE);;
                                gtk_widget_set_sensitive(datos->buttonsus[p][py-1], FALSE);
                            }
                            else
                                gtk_widget_set_sensitive(datos->buttonsus[p][py-1], FALSE);
                        }
                    }
                    if (px!=0) //4 5 6
                    {
                        if (py==0) //4
                        {
                            if(n==0)//primera validación
                            {
                                gtk_widget_set_sensitive(datos->buttonsus[p-1][py], FALSE);
                                gtk_widget_set_sensitive(datos->buttonsus[p][py+1], FALSE);
                            }
                            else
                            {
                                if (n==barco-1)//ultima
                                {
                                    if (p==9)//justo
                                        gtk_widget_set_sensitive(datos->buttonsus[p][py+1], FALSE);
                                    else // noj
                                    {
                                        gtk_widget_set_sensitive(datos->buttonsus[p][py+1], FALSE);
                                        gtk_widget_set_sensitive(datos->buttonsus[p+1][py], FALSE);
                                    }
                                }
                                else
                                    gtk_widget_set_sensitive(datos->buttonsus[p][py+1], FALSE);
                            }
                        }
                        else
                        {
                            if (py==9) //6
                            {
                                if(p==px)//primera validación
                                {
                                    gtk_widget_set_sensitive(datos->buttonsus[p][py-1], FALSE);
                                    gtk_widget_set_sensitive(datos->buttonsus[p-1][py], FALSE);
                                }
                                else
                                {
                                    if (n==barco-1)//Ultima
                                    {
                                        if(p==9)//queda justo
                                            gtk_widget_set_sensitive(datos->buttonsus[p][py-1], FALSE);
                                        else //no queda justo
                                        {
                                            gtk_widget_set_sensitive(datos->buttonsus[p][py-1], FALSE);
                                            gtk_widget_set_sensitive(datos->buttonsus[p+1][py], FALSE);
                                        }
                                    }
                                    else//inter
                                        gtk_widget_set_sensitive(datos->buttonsus[p][py-1], FALSE);
                                }
                            }

                            else //5
                            {
                                if(n==0)//primera validación
                                {
                                    gtk_widget_set_sensitive(datos->buttonsus[p-1][py], FALSE);
                                    gtk_widget_set_sensitive(datos->buttonsus[p][py+1], FALSE);
                                    gtk_widget_set_sensitive(datos->buttonsus[p][py-1], FALSE);

                                }
                                else
                                {
                                    if (n==barco-1)//Ultima
                                    {
                                        if(p==9)//queda justo
                                        {
                                            gtk_widget_set_sensitive(datos->buttonsus[p][py+1], FALSE);
                                            gtk_widget_set_sensitive(datos->buttonsus[p][py-1], FALSE);
                                        }
                                        else //no queda justo
                                        {
                                            gtk_widget_set_sensitive(datos->buttonsus[p][py+1], FALSE);
                                            gtk_widget_set_sensitive(datos->buttonsus[p][py-1], FALSE);
                                            gtk_widget_set_sensitive(datos->buttonsus[p+1][py], FALSE);

                                        }
                                    }
                                    else//inter
                                    {
                                        gtk_widget_set_sensitive(datos->buttonsus[p][py+1], FALSE);
                                        gtk_widget_set_sensitive(datos->buttonsus[p][py-1], FALSE);
                                    }
                                }
                            }
                        }

                    }
                    n++;
                    p++;
                }
            }
            if(datos->pos==1)
            {
                p=py;
                while(n<barco)
                {
                    datos->us[px][p]='1';
                    datos->orgus[px][p]='1';
                    gtk_button_set_label(GTK_BUTTON(datos->buttonsus[px][p]), "B");
                    gtk_widget_set_sensitive(datos->buttonsus[px][p], FALSE);
                    if (py==0) // 7 8 9
                    {
                        if (px==0)//7
                        {
                            if (n==barco-1)//ultimo
                            {
                                gtk_widget_set_sensitive(datos->buttonsus[px][p+1], FALSE);
                                gtk_widget_set_sensitive(datos->buttonsus[px+1][p], FALSE);
                            }
                            else
                                gtk_widget_set_sensitive(datos->buttonsus[px+1][p], FALSE);
                        }
                        else
                        {
                            if (px==9)//9
                            {
                                if (n==barco-1)
                                {
                                    gtk_widget_set_sensitive(datos->buttonsus[px][p+1], FALSE);
                                    gtk_widget_set_sensitive(datos->buttonsus[px-1][p], FALSE);
                                }
                                else
                                    gtk_widget_set_sensitive(datos->buttonsus[px-1][p], FALSE);
                            }
                            else // 8
                            {
                                if (n==barco-1)
                                {
                                    gtk_widget_set_sensitive(datos->buttonsus[px+1][p], FALSE);
                                    gtk_widget_set_sensitive(datos->buttonsus[px-1][p], FALSE);
                                    gtk_widget_set_sensitive(datos->buttonsus[px][p+1], FALSE);
                                }
                                else
                                {
                                    gtk_widget_set_sensitive(datos->buttonsus[px+1][p], FALSE);
                                    gtk_widget_set_sensitive(datos->buttonsus[px-1][p], FALSE);
                                }
                            }
                        }
                    }
                    if (py!=0) // 10 11 12
                    {
                        if (px==0) //10
                        {
                            if (n==0) //primero
                            {
                                gtk_widget_set_sensitive(datos->buttonsus[px][p-1], FALSE);
                                gtk_widget_set_sensitive(datos->buttonsus[px+1][p], FALSE);
                            }
                            else
                            {
                                if (n==barco-1)//ultimo
                                {
                                    if (p==9)
                                        gtk_widget_set_sensitive(datos->buttonsus[px+1][p], FALSE);
                                    else
                                    {
                                        gtk_widget_set_sensitive(datos->buttonsus[px+1][p], FALSE);
                                        gtk_widget_set_sensitive(datos->buttonsus[px][p+1], FALSE);
                                    }
                                }
                                else
                                    gtk_widget_set_sensitive(datos->buttonsus[px+1][p], FALSE);
                            }
                        }
                        else
                        {
                            if (px==9)//12
                            {
                                if (n==0) //primero
                                {
                                    gtk_widget_set_sensitive(datos->buttonsus[px-1][p], FALSE);
                                    gtk_widget_set_sensitive(datos->buttonsus[px][p-1], FALSE);
                                }
                                else
                                {
                                    if (n==barco-1)//ultimo
                                    {
                                        if (p==9)
                                        {
                                            gtk_widget_set_sensitive(datos->buttonsus[px-1][p], FALSE);
                                        }
                                        else
                                        {
                                            gtk_widget_set_sensitive(datos->buttonsus[px][p+1], FALSE);
                                            gtk_widget_set_sensitive(datos->buttonsus[px-1][p], FALSE);
                                        }
                                    }
                                    else
                                        gtk_widget_set_sensitive(datos->buttonsus[px-1][p], FALSE);

                                }
                            }
                            else//11
                            {
                                if(n==0)
                                {
                                    gtk_widget_set_sensitive(datos->buttonsus[px+1][p], FALSE);
                                    gtk_widget_set_sensitive(datos->buttonsus[px-1][p], FALSE);
                                    gtk_widget_set_sensitive(datos->buttonsus[px][p-1], FALSE);
                                }
                                else
                                {
                                    if(n==barco-1)
                                    {
                                        if (p==9)
                                        {
                                            gtk_widget_set_sensitive(datos->buttonsus[px+1][p], FALSE);
                                            gtk_widget_set_sensitive(datos->buttonsus[px-1][p], FALSE);
                                        }
                                        else
                                        {
                                            gtk_widget_set_sensitive(datos->buttonsus[px+1][p], FALSE);
                                            gtk_widget_set_sensitive(datos->buttonsus[px-1][p], FALSE);
                                            gtk_widget_set_sensitive(datos->buttonsus[px][p+1], FALSE);
                                        }
                                    }
                                    else
                                    {
                                        gtk_widget_set_sensitive(datos->buttonsus[px+1][p], FALSE);
                                        gtk_widget_set_sensitive(datos->buttonsus[px-1][p], FALSE);
                                    }
                                }
                            }
                        }
                    }
                    n++;
                    p++;
                }
            }
            datos->puestos++;
        }

    }
}

void PonBarcosComp(tipoboton *estructura) //pone los barcos solo lo cambia en el arreglo
{
    int x, y, pos, i=0, bandera, barco=5, j, h;
    srand(time(NULL));
    do
    {
        pos=0+rand()%((1+1)-0);
        x=0+rand()%((9+0)-0);
        y=0+rand()%((9+0)-0);
        bandera=0;
        if(estructura->comp[x][y]!='*') //validamos que sea agua donde lo va a poner
            bandera=1;
        if(pos==0)
        {
            for(j=x; j<x+barco; j++)
            {
                if (y-1<0) //4 8 1
                {
                    if (x-1<0) //1
                    {
                        if(j==(x+barco)-1)
                        {
                            if((estructura->comp[j+1][y]=='1')||(estructura->comp[j][y+1]!='*'))
                                bandera=1;
                        }
                        else
                        {
                            if(estructura->comp[j][y+1]!='*')
                                bandera=1;
                        }
                    }
                    else
                    {
                        if(j>9) //validamos que no se salga del arreglo
                            bandera=1;
                        if (j==(x+barco)-1)
                        {
                            if (j+1>9)
                            {
                                if(estructura->comp[j][y+1]!='*')
                                    bandera=1;
                            }
                            else
                            {
                                if ((estructura->comp[j+1][y]!='*')||(estructura->comp[j][y+1]!='*'))
                                    bandera=1;
                            }
                        }
                        else
                        {
                            if (j==x)
                            {
                                if ((estructura->comp[j-1][y]!='*')||(estructura->comp[j][y+1]!='*'))
                                    bandera=1;
                            }
                            else
                            {
                                if (estructura->comp[j][y+1]!='*')
                                    bandera=1;
                            }
                        }
                    }
                }
                if (x-1<0)//3 2
                {
                    if (y+1>9)
                    {
                        if (j==(x+barco)-1)
                        {
                            if ((estructura->comp[j+1][y]!='*')||(estructura->comp[j][y-1]!='*'))
                                bandera=1;
                        }
                        else
                        {
                            if (estructura->comp[j][y+1]!='*')
                                bandera=1;
                        }
                    }
                    else
                    {
                        if (j==(x+barco)-1)
                        {
                            if ((estructura->comp[j][y+1]!='*')||(estructura->comp[j][y-1]!='*')||(estructura->comp[j+1][y]!='*'))
                                bandera=1;
                        }
                        else
                        {
                            if ((estructura->comp[j][y-1]!='*')||(estructura->comp[j][y+1]!='*'))
                                bandera=1;
                        }
                    }
                }
                if ((x-1>=0)&&(y-1>=0))//5 9 7 6
                {
                    if (j>9)
                        bandera=1;
                    if (y+1<=9) // 5 9
                    {
                        if (j==(x+barco)-1)
                        {
                            if (j+1>9)
                            {
                                if ((estructura->comp[j][y-1]!='*')||(estructura->comp[j][y+1]!='*'))
                                    bandera=1;
                            }
                            else
                            {
                                if ((estructura->comp[j+1][y]!='*')||(estructura->comp[j][y+1]!='*')||(estructura->comp[j][y-1]!='*'))
                                    bandera=1;
                            }
                        }
                        else
                        {
                            if (j==x)
                            {
                                if ((estructura->comp[j-1][y]!='*')||(estructura->comp[j][y+1]!='*')||(estructura->comp[j][y-1]!='*'))
                                    bandera=1;
                            }
                            else
                            {
                                if ((estructura->comp[j][y+1]!='*')||(estructura->comp[j][y-1]!='*'))

                                    bandera=1;
                            }
                        }
                    }
                    if (y+1>9) //7 6 ----
                    {
                        if (j==(x+barco)-1)
                        {
                            if (j+1>9)
                            {
                                if (estructura->comp[j][y-1]!='*')
                                    bandera=1;
                            }
                            else
                            {
                                if ((estructura->comp[j+1][y]!='*')||(estructura->comp[j][y-1]!='*'))
                                    bandera=1;
                            }
                        }
                        else
                        {
                            if (j==x)
                            {
                                if ((estructura->comp[j-1][y]!='*')||(estructura->comp[j][y-1]!='*'))
                                    bandera=1;
                            }
                            else
                            {
                                if (estructura->comp[j][y-1]!='*')
                                    bandera=1;
                            }
                        }
                    }
                }
            }
        }
        else // pos==1
        {
            for(j=y; j<y+barco; j++)
            {
                if (y-1<0) // 10  13  16---------
                {
                    if (x+1>9) //16--
                    {
                        if (j==(y+barco)-1)
                        {
                            if ((estructura->comp[x][j+1]!='*')||(estructura->comp[x-1][j]!='*'))
                                bandera=1;
                        }
                        else
                        {
                            if (estructura->comp[x-1][j]!='*')
                                bandera=1;

                        }
                    }
                    else
                    {
                        if(x-1<0) //10--
                        {
                            if (j==(y+barco)-1)
                            {
                                if ((estructura->comp[x+1][j]!='*')||(estructura->comp[x][j+1]!='*'))
                                    bandera=1;
                            }
                            else
                            {
                                if (estructura->comp[x+1][j]!='*')
                                    bandera=1;
                            }
                        }
                        else //13
                        {
                            if (j==(y+barco)-1)
                            {
                                if ((estructura->comp[x-1][j]!='*')||(estructura->comp[x+1][j]!='*')||(estructura->comp[x][j+1]!='*'))
                                    bandera=1;
                            }
                            else
                            {
                                if ((estructura->comp[x+1][j]!='*')||(estructura->comp[x-1][j]!='*'))
                                    bandera=1;
                            }
                        }
                    }
                }
                if ((x-1<0)&&(y-1>0))
                {
                    if (j>9)
                        bandera=1;
                    if (j==(y+barco)-1)
                    {
                        if (j+1>9)
                        {
                            if (estructura->comp[x+1][j]!='*')
                                bandera=1;
                        }
                        else
                        {
                            if ((estructura->comp[x+1][j]!='*')||(estructura->comp[x][j+1]!='*'))
                                bandera=1;
                        }
                    }
                    else
                    {
                        if (j==y)
                        {
                            if ((estructura->comp[x][j-1]!='*')||(estructura->comp[x+1][j]!='*'))
                                bandera=1;
                        }
                        else
                        {
                            if (estructura->comp[x+1][j]!='*')
                                bandera=1;

                        }
                    }
                }

                if ((x+1<=9)&&(x-1>=0)&&(y-1>=0))//14  15---
                {
                    if (j>9)
                        bandera=1;
                    if (j==(y+barco)-1)
                    {
                        if (j+1>9)
                        {
                            if ((estructura->comp[x+1][j]!='*')||(estructura->comp[x-1][j]!='*'))
                                bandera=1;
                        }
                        else
                        {
                            if ((estructura->comp[x-1][j]!='*')||(estructura->comp[x][j+1]!='*')||(estructura->comp[x+1][j]!='*'))
                                bandera=1;
                        }
                    }
                    if (j==y)
                    {
                        if ((estructura->comp[x+1][j]!='*')||(estructura->comp[x-1][j]!='*')||(estructura->comp[x][j-1]!='*'))
                            bandera=1;
                    }
                    else
                    {
                        if ((estructura->comp[x+1][j]!='*')||(estructura->comp[x-1][j]!='*'))
                            bandera=1;
                    }
                }
                if ((x+1>9)&&(y-1>0))//17 18
                {
                    if (j>9)
                        bandera=1;

                    if (j==(y+barco)-1)
                    {
                        if (j+1>9)
                        {
                            if (estructura->comp[x-1][j]!='*')
                                bandera=1;
                        }
                        else
                        {
                            if ((estructura->comp[x-1][j]!='*')||(estructura->comp[x][j+1]!='*'))
                                bandera=1;
                        }
                    }
                    if (j==y)
                    {
                        if ((estructura->comp[x-1][j]!='*')||(estructura->comp[x][j-1]!='*'))
                            bandera=1;
                    }
                    else
                    {
                        if (estructura->comp[x-1][j]!='*')
                            bandera=1;
                    }
                }
            }
        }

        if(bandera==0)
        {
            if(i==0)
            {
                barco=4;
                if(pos==0)
                {
                    estructura->comp[x][y]='1';
                    estructura->comp[x+1][y]='1';
                    estructura->comp[x+2][y]='1';
                    estructura->comp[x+3][y]='1';
                    estructura->comp[x+4][y]='1';
                    estructura->orgcomp[x][y]='1';
                    estructura->orgcomp[x+1][y]='1';
                    estructura->orgcomp[x+2][y]='1';
                    estructura->orgcomp[x+3][y]='1';
                    estructura->orgcomp[x+4][y]='1';
                }
                if(pos==1)
                {
                    estructura->comp[x][y]='1';
                    estructura->comp[x][y+1]='1';
                    estructura->comp[x][y+2]='1';
                    estructura->comp[x][y+3]='1';
                    estructura->comp[x][y+4]='1';
                    estructura->orgcomp[x][y]='1';
                    estructura->orgcomp[x][y+1]='1';
                    estructura->orgcomp[x][y+2]='1';
                    estructura->orgcomp[x][y+3]='1';
                    estructura->orgcomp[x][y+4]='1';
                }
            }
            if(i==1)
            {
                barco=3;
                if(pos==0)
                {
                    estructura->comp[x][y]='1';
                    estructura->comp[x+1][y]='1';
                    estructura->comp[x+2][y]='1';
                    estructura->comp[x+3][y]='1';
                    estructura->orgcomp[x][y]='1';
                    estructura->orgcomp[x+1][y]='1';
                    estructura->orgcomp[x+2][y]='1';
                    estructura->orgcomp[x+3][y]='1';
                }
                if(pos==1)
                {
                    estructura->comp[x][y]='1';
                    estructura->comp[x][y+1]='1';
                    estructura->comp[x][y+2]='1';
                    estructura->comp[x][y+3]='1';
                    estructura->orgcomp[x][y]='1';
                    estructura->orgcomp[x][y+1]='1';
                    estructura->orgcomp[x][y+2]='1';
                    estructura->orgcomp[x][y+3]='1';
                }
            }
            if((i==2)||(i==3))
            {
                if(i==3)
                    barco=2;
                if(pos==0)
                {
                    estructura->comp[x][y]='1';
                    estructura->comp[x+1][y]='1';
                    estructura->comp[x+2][y]='1';
                    estructura->orgcomp[x][y]='1';
                    estructura->orgcomp[x+1][y]='1';
                    estructura->orgcomp[x+2][y]='1';
                }
                if(pos==1)
                {
                    estructura->comp[x][y]='1';
                    estructura->comp[x][y+1]='1';
                    estructura->comp[x][y+2]='1';
                    estructura->orgcomp[x][y]='1';
                    estructura->orgcomp[x][y+1]='1';
                    estructura->orgcomp[x][y+2]='1';
                }
            }
            if(i==4)
            {
                if(pos==0)
                {
                    estructura->comp[x][y]='1';
                    estructura->comp[x+1][y]='1';
                    estructura->orgcomp[x][y]='1';
                    estructura->orgcomp[x+1][y]='1';
                }
                if(pos==1)
                {
                    estructura->comp[x][y]='1';
                    estructura->comp[x][y+1]='1';
                    estructura->orgcomp[x][y]='1';
                    estructura->orgcomp[x][y+1]='1';
                }
            }
        }
        if (bandera==0)
            i++;
    }while(i<5);
}

void Inserta(tipoboton *datos)
{
    tipoboton *temp2, *temp;
    int i, j;
    temp=(tipoboton*)malloc(sizeof(tipoboton));

    temp->pos=datos->pos;
    temp->x=datos->x;
    temp->y=datos->y;
    temp->nx=datos->nx;
    temp->ny=datos->ny;
    temp->pego=datos->pego;
    temp->horizontal=datos->horizontal;
    temp->vertical=datos->vertical;
    temp->arribaus=datos->arribaus;
    temp->abajous=datos->abajous;
    temp->derus=datos->derus;
    temp->izqus=datos->izqus;
    temp->arribacomp=datos->arribacomp;
    temp->abajocomp=datos->abajocomp;
    temp->dercomp=datos->dercomp;
    temp->izqcomp=datos->izqcomp;
    temp->hundido=datos->hundido;
    temp->turno=datos->turno;
    temp->cuantoscomp=datos->cuantoscomp;
    temp->cuantosus=datos->cuantosus;
    temp->ganador=datos->ganador;
    temp->tx=datos->tx;
    temp->ty=datos->ty;
    temp->puestos=datos->puestos;
    strcpy(temp->NombreUs, datos->NombreUs);

    for(i=0; i<10; i++)
    {
        for(j=0; j<10; j++)
        {
            temp->us[i][j]=datos->us[i][j];
            temp->comp[i][j]=datos->comp[i][j];
        }
    }

    temp->sig=NULL;
    if(datos->Inicio==NULL)
    {
        datos->Inicio=temp;
    }
    else
    {
        temp2=datos->Inicio;
        while(temp2->sig!=NULL)
        {
            temp2=temp2->sig;
        }
        temp2->sig=temp;
    }
}

void GuardarPartida(GtkWidget *button, gpointer data)
{
    FILE *Archivo;
    int i, j;
    GtkWidget *dialog;
    GtkFileChooserAction action=GTK_FILE_CHOOSER_ACTION_SAVE;
    gint res;
    tipoboton *temp;
    tipoboton *datos=(tipoboton*)data;

    temp=datos->Inicio;
    dialog=gtk_file_chooser_dialog_new("Guardar Arhivo", NULL, action, "Cancelar", GTK_RESPONSE_CANCEL, "Guardar", GTK_RESPONSE_ACCEPT, NULL);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *NombreArchivo;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        NombreArchivo = gtk_file_chooser_get_filename (chooser);
        Archivo=fopen(NombreArchivo, "wt");
        while(temp!=NULL)
        {
            fprintf(Archivo, "%d\n", temp->pos);
            fprintf(Archivo, "%d\n", temp->x);
            fprintf(Archivo, "%d\n", temp->y);
            fprintf(Archivo, "%d\n", temp->nx);
            fprintf(Archivo, "%d\n", temp->ny);
            fprintf(Archivo, "%d\n", temp->pego);
            fprintf(Archivo, "%d\n", temp->horizontal);
            fprintf(Archivo, "%d\n", temp->vertical);
            fprintf(Archivo, "%d\n", temp->arribaus);
            fprintf(Archivo, "%d\n", temp->abajous);
            fprintf(Archivo, "%d\n", temp->derus);
            fprintf(Archivo, "%d\n", temp->izqus);
            fprintf(Archivo, "%d\n", temp->arribacomp);
            fprintf(Archivo, "%d\n", temp->abajocomp);
            fprintf(Archivo, "%d\n", temp->dercomp);
            fprintf(Archivo, "%d\n", temp->izqcomp);
            fprintf(Archivo, "%d\n", temp->hundido);
            fprintf(Archivo, "%d\n", temp->cuantoscomp);
            fprintf(Archivo, "%d\n", temp->cuantosus);
            fprintf(Archivo, "%d\n", temp->ganador);
            fprintf(Archivo, "%d\n", temp->tx);
            fprintf(Archivo, "%d\n", temp->ty);
            fprintf(Archivo, "%d\n", temp->turno);
            fprintf(Archivo, "%d\n", temp->puestos);
            fprintf(Archivo, "%s\n", temp->NombreUs);
            for(i=0; i<10; i++)
            {
                for(j=0; j<10; j++)
                {
                    fprintf(Archivo, "%c", temp->us[i][j]);
                }
                fprintf(Archivo, "\n");
            }
            for(i=0; i<10; i++)
            {
                for(j=0; j<10; j++)
                {
                    fprintf(Archivo, "%c", temp->comp[i][j]);
                }
                fprintf(Archivo, "\n");
            }
            temp=temp->sig;
        }
        fclose(Archivo);
    }
    gtk_widget_destroy(dialog);
}

void LeerArchivo(GtkWidget *button, gpointer data)
{
    FILE *Archivo;
    GtkWidget *dialog;
    GtkFileChooserAction action=GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    tipoboton *temp, *temp2;
    tipoboton *datos=(tipoboton*)data;
    int i, j;
    dialog=gtk_file_chooser_dialog_new("Abrir Arhivo", NULL, action, "Cancelar", GTK_RESPONSE_CANCEL, "Abrir", GTK_RESPONSE_ACCEPT, NULL);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *NombreArchivo;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        NombreArchivo = gtk_file_chooser_get_filename (chooser);
        Archivo=fopen(NombreArchivo, "rt");
        if(Archivo!=NULL)
        {
            while(!feof(Archivo))
            {
                temp=(tipoboton*)malloc(sizeof(tipoboton));
                fscanf(Archivo, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%s\n", &temp->pos, &temp->x, &temp->y, &temp->nx, &temp->ny, &temp->pego, &temp->horizontal, &temp->vertical, &temp->arribaus, &temp->abajous, &temp->derus, &temp->izqus, &temp->arribacomp, &temp->abajocomp, &temp->dercomp, &temp->izqcomp, &temp->hundido, &temp->cuantoscomp, &temp->cuantosus, &temp->ganador, &temp->tx, &temp->ty, &temp->turno, &temp->puestos, temp->NombreUs);
                strcpy(datos->NombreUs, temp->NombreUs);
                for(i=0; i<10; i++)
                {
                    for(j=0; j<10; j++)
                    {

                        fscanf(Archivo, "%c", &temp->us[i][j]);

                    }
                    fscanf (Archivo,"\n");
                }

                for(i=0; i<10; i++)
                {
                    for(j=0; j<10; j++)
                    {
                        fscanf(Archivo, "%c", &temp->comp[i][j]);
                    }
                    fscanf (Archivo,"\n");
                }
                temp->sig=NULL;
                if (datos->Inicio==NULL)
                    datos->Inicio=temp;
                else
                {
                    temp2=datos->Inicio;
                    while(temp2->sig!=NULL)
                        temp2=temp2->sig;
                    temp2->sig=temp;
                }
            }
            fclose(Archivo);
            datos->ap=datos->Inicio;
            VentanaReanudar(datos);
        }
    }
    gtk_widget_destroy(dialog);
}

void VentanaReanudar(tipoboton *datos)
{
    GdkColor azul;
    GdkColor rojo;
    GdkColor verde;
    GtkWidget *button3, *iniciar, *siguiente;
    GtkWidget *hbox1, *hbox2, *Vbox1, *Vbox2, *Hbox1, *Hbox2;
    GtkWidget *vbox1, *label;
    const gchar *text;
    char text2[20];
    int i, j;


    gdk_color_parse("#088A08", &verde);
    gdk_color_parse("#0174DF", &azul);
    gdk_color_parse("#DF0101", &rojo);

    gtk_widget_hide_all(datos->optionw);
    datos->reanudarw=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(datos->reanudarw),600,300);
    gtk_container_border_width(GTK_CONTAINER(datos->reanudarw), 5);
    gtk_signal_connect(GTK_OBJECT(datos->reanudarw),"destroy", GTK_SIGNAL_FUNC(Destroy), datos);

    Vbox1=gtk_vbox_new(FALSE, 0);
    strcpy(text2, datos->NombreUs);
    label=gtk_label_new(text2);
    gtk_box_pack_start(GTK_BOX(Vbox1), label, TRUE, TRUE, 0);

    for(i=0; i<10; i++)
    {
        hbox1=gtk_hbox_new(TRUE, 0);
        for(j=0; j<10; j++)
        {
            datos->buttonsus[i][j]=gtk_button_new();
            gtk_box_pack_start(GTK_BOX(hbox1), datos->buttonsus[i][j], TRUE, TRUE, 0);
        }
        gtk_box_pack_start(GTK_BOX(Vbox1), hbox1, TRUE, TRUE, 0);
    }

    Vbox2=gtk_vbox_new(FALSE, 0);
    label=gtk_label_new("COMPUTADORA");
    gtk_box_pack_start(GTK_BOX(Vbox2), label, TRUE, TRUE, 0);
    for(i=0; i<10; i++) //arreglo de botones de la compu
    {
        hbox2=gtk_hbox_new(TRUE, 0);
        for(j=0; j<10; j++)
        {
            datos->buttonscomp[i][j]=gtk_button_new();
            gtk_box_pack_start(GTK_BOX(hbox2), datos->buttonscomp[i][j], TRUE, TRUE, 0);
        }
        gtk_box_pack_start(GTK_BOX(Vbox2), hbox2, TRUE, TRUE, 0);
    }

    Hbox1=gtk_hbox_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX(Hbox1), Vbox1, TRUE, TRUE, 20);
    gtk_box_pack_start(GTK_BOX(Hbox1), Vbox2, TRUE, TRUE, 20);

    Hbox2=gtk_hbox_new(TRUE, 0);
    siguiente=gtk_button_new_with_label("Siguiente jugada");
    iniciar=gtk_button_new_with_label("Continuar juego");

    gtk_signal_connect(GTK_OBJECT(siguiente), "clicked",GTK_SIGNAL_FUNC(Siguiente), datos);
    gtk_signal_connect(GTK_OBJECT(iniciar), "clicked",GTK_SIGNAL_FUNC(VentanaJuega), datos);
    gtk_box_pack_start(GTK_BOX(Hbox2), siguiente, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(Hbox2), iniciar, FALSE, FALSE, 0);

    vbox1=gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox1), Hbox1, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(vbox1), Hbox2, FALSE, FALSE, 0);

    gtk_window_set_title( GTK_WINDOW(datos->reanudarw), "BATALLA NAVAL" );

    gtk_container_add(GTK_CONTAINER(datos->reanudarw), vbox1);
    gtk_widget_show_all(datos->reanudarw);
}

void Siguiente(GtkWidget *button, gpointer data)
{
    tipoboton *datos=(tipoboton *)data;
    GdkColor azul;
    GdkColor rojo;
    GdkColor verde;
    int i, j;
    tipoboton *temp=datos->ap;
    gdk_color_parse("#088A08", &verde);
    gdk_color_parse("#0174DF", &azul);
    gdk_color_parse("#DF0101", &rojo);
    if(temp!=NULL)
    {
        for(i=0; i<10; i++)
        {
            for(j=0; j<10; j++)
            {
                if(temp->comp[i][j]=='h')
                    gtk_widget_modify_bg(datos->buttonscomp[i][j], GTK_STATE_NORMAL, &rojo);
                if(temp->comp[i][j]=='a')
                    gtk_widget_modify_bg(datos->buttonscomp[i][j], GTK_STATE_NORMAL, &azul);
            }
        }
        for(i=0; i<10; i++)
        {
            for(j=0; j<10; j++)
            {
                if(temp->us[i][j]=='1')
                    gtk_widget_modify_bg(datos->buttonsus[i][j], GTK_STATE_NORMAL, &verde);
                if(temp->us[i][j]=='a')
                    gtk_widget_modify_bg(datos->buttonsus[i][j], GTK_STATE_NORMAL, &azul);
                if(temp->us[i][j]=='h')
                    gtk_widget_modify_bg(datos->buttonsus[i][j], GTK_STATE_NORMAL, &rojo);
            }
        }
        datos->pos=temp->pos;
        datos->x=temp->x;
        datos->y=temp->y;
        datos->nx=temp->nx;
        datos->ny=temp->ny;
        datos->pego=temp->pego;
        datos->horizontal=temp->horizontal;
        datos->vertical=temp->vertical;
        datos->arribaus=temp->arribaus;
        datos->abajous=temp->abajous;
        datos->derus=temp->derus;
        datos->izqus=temp->izqus;
        datos->arribacomp=temp->arribacomp;
        datos->abajocomp=temp->abajocomp;
        datos->dercomp=temp->dercomp;
        datos->izqcomp=temp->izqcomp;
        datos->hundido=temp->hundido;
        datos->cuantoscomp=temp->cuantoscomp;
        datos->cuantosus=temp->cuantosus;
        datos->ganador=temp->ganador;
        datos->tx=temp->tx;
        datos->ty=temp->ty;
        datos->turno=temp->turno;
        datos->puestos=temp->puestos;
        strcpy(datos->NombreUs, temp->NombreUs);
        for(i=0; i<10; i++)
        {
            for(j=0; j<10; j++)
            {
                datos->us[i][j]=temp->us[i][j];
                datos->comp[i][j]=temp->comp[i][j];
            }
        }
        temp=temp->sig;
        datos->ap=temp;
    }
    else
    {
        gtk_widget_set_sensitive(button, FALSE);
    }
}

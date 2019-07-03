#include <gtk/gtk.h>
#include "windowParam.h"

#include <cstdio>
#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

const char* default_cannyl_str = "150";
const char* default_cannyh_str = "200";

const char* default_ZerBgrL_str = "10";
const char* default_ZerGrrH_str = "50";

const char* default_cannyCore_str = "3";
const char* default_drawColor_str = "255";
const char* default_neighborSize_str = "7";

const char* default_HScale_str = "4";
const char* default_VScale_str = "4";

const bool default_useMedium = true;

ParamSet *_params;
GtkWidget* windowParam;

GtkWidget *Vlayout_main;
GtkWidget *Hlayout_l1;
GtkWidget *Hlayout_l2;
GtkWidget *Hlayout_l3;
GtkWidget *Hlayout_l4;
GtkWidget *Hlayout_l5;

GtkWidget *tableLayout_main;

GtkWidget *btn_apply;
GtkWidget *btn_cancel;
GtkWidget *btn_default;

GtkWidget *labe_cannyL;
GtkWidget *labe_cannyH;
GtkWidget *labe_cannyCore;
GtkWidget *labe_ZerBgrL;
GtkWidget *labe_ZerBgrH;
GtkWidget *label_drawColor;
GtkWidget *label_neighborSize;
GtkWidget *label_VScale;
GtkWidget *label_HScale;

GtkWidget *entry_cannyL;
GtkWidget *entry_cannyH;
GtkWidget *entry_cannyCore;
GtkWidget *entry_ZerBgrL;
GtkWidget *entry_ZerBgrH;
GtkWidget *entry_drawColor;
GtkWidget *entry_neighborSize;
GtkWidget *entry_VScale;
GtkWidget *entry_HScale;


GtkWidget *checkBox_meida;

void Initial_WindowParam(ParamSet *ext_params){

    windowParam = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(windowParam),"参数设置");
    gtk_window_set_default_size(GTK_WINDOW(windowParam),200,200);
    gtk_container_set_border_width(GTK_CONTAINER(windowParam),10);

    //layout initial
    tableLayout_main = gtk_table_new(9,6,TRUE);
    gtk_container_add(GTK_CONTAINER(windowParam),tableLayout_main);

    //widgets initial
    btn_apply = gtk_button_new_with_label("应用修改");
    btn_cancel = gtk_button_new_with_label("取消修改");
    btn_default = gtk_button_new_with_label("恢复默认");

    checkBox_meida = gtk_check_button_new_with_label("是否中值滤波");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkBox_meida),TRUE);
    g_signal_connect(GTK_OBJECT(checkBox_meida),"toggled",G_CALLBACK(on_checkBox_meida_toggle),NULL);

    entry_cannyL = gtk_entry_new();
    entry_cannyH = gtk_entry_new();
    entry_cannyCore = gtk_entry_new();
    entry_ZerBgrL = gtk_entry_new();
    entry_ZerBgrH = gtk_entry_new();
    entry_drawColor = gtk_entry_new();
    entry_neighborSize = gtk_entry_new();
    entry_VScale = gtk_entry_new();
    entry_HScale = gtk_entry_new();
//高低
    labe_cannyL = gtk_label_new("Canny低阈值");
    labe_cannyH = gtk_label_new("Canny高阈值");
    labe_cannyCore = gtk_label_new("Canny核尺寸(odd)");
    labe_ZerBgrL = gtk_label_new("边缘背景低阈值");
    labe_ZerBgrH = gtk_label_new("边缘背景高阈值");
    label_drawColor = gtk_label_new("画笔颜色 [0-255]");
    label_neighborSize = gtk_label_new("边缘邻域尺寸(odd)");
    label_VScale = gtk_label_new("垂直缩放尺度");
    label_HScale = gtk_label_new("水平缩放尺度");


    //connect signals and handler
    g_signal_connect(G_OBJECT(btn_apply),"clicked",G_CALLBACK(on_btn_apply_clicked),(gpointer)NULL);
    g_signal_connect(G_OBJECT(btn_cancel),"clicked",G_CALLBACK(on_btn_cancel_clicked),(gpointer)NULL);
    g_signal_connect(G_OBJECT(btn_default),"clicked",G_CALLBACK(on_btn_default_clicked),(gpointer)NULL);

    //add widgets to layouts

    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),labe_cannyL, 0,3,0,1);
    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),entry_cannyL, 3,6,0,1);

    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),labe_cannyH, 0,3,1,2);
    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),entry_cannyH, 3,6,1,2);

    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),labe_cannyCore, 0,3,2,3);
    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),entry_cannyCore, 3,6,2,3);

    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),labe_ZerBgrL, 0,3,3,4);
    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),entry_ZerBgrL, 3,6,3,4);

    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),labe_ZerBgrH, 0,3,4,5);
    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),entry_ZerBgrH, 3,6,4,5);

    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),label_neighborSize, 0,3,5,6);
    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),entry_neighborSize, 3,6,5,6);

    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),label_drawColor, 0,3,6,7);
    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),entry_drawColor, 3,6,6,7);

    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),label_VScale, 0,3,7,8);
    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),entry_VScale, 3,6,7,8);

    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),label_HScale, 0,3,8,9);
    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),entry_HScale, 3,6,8,9);

    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),checkBox_meida, 3,6,9,10);

    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),btn_apply, 0,2,10,11);
    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),btn_cancel, 2,4,10,11);
    gtk_table_attach_defaults(GTK_TABLE(tableLayout_main),btn_default, 4,6,10,11);

    //params setting
    _params = ext_params;
    Use_DefaultParams(_params);
    Fill_DefaultParams();

}

void Show_WindowParam()
{
    FillParamsToView(_params);
    gtk_widget_show_all(windowParam);
}

void Hide_WindowParam(){
    ;
}


/*
ZerResult CalDistanceSquare(Mat& matRoi, double rto = MMPERPIXEL,
        int drawColor = WHITE, bool ifmedianBlur=YESMEDIANBLUR,
        int cannyL = 150, int cannyH = 200, int cannyCore = 3,
        int nbsize = N, Point2i VHtime = Point2i(4, 4),
        int ZerBgrL = 10, int ZerBgrH = 50

    );
ZerResultCir CalDistanceCircle(Mat& matRoi, double rto = MMPERPIXEL,
        int drawColor = WHITE, bool ifmedianBlur = YESMEDIANBLUR,
        int cannyL = 150, int cannyH = 200, int cannyCore = 3,
        int nbsize = N, Point2i VHtime = Point2i(4, 4),
        int ZerBgrL = 10, int ZerBgrH = 50

    );
*/


void Fill_DefaultParams(){
    gtk_entry_set_text(GTK_ENTRY(entry_cannyL), default_cannyl_str);
    gtk_entry_set_text(GTK_ENTRY(entry_cannyH), default_cannyh_str);
    gtk_entry_set_text(GTK_ENTRY(entry_ZerBgrL), default_ZerBgrL_str);
    gtk_entry_set_text(GTK_ENTRY(entry_ZerBgrH), default_ZerGrrH_str);
    gtk_entry_set_text(GTK_ENTRY(entry_cannyCore), default_cannyCore_str);
    gtk_entry_set_text(GTK_ENTRY(entry_drawColor), default_drawColor_str);
    gtk_entry_set_text(GTK_ENTRY(entry_neighborSize), default_neighborSize_str);
    gtk_entry_set_text(GTK_ENTRY(entry_VScale), default_VScale_str);
    gtk_entry_set_text(GTK_ENTRY(entry_HScale), default_HScale_str);
}

void Use_DefaultParams(ParamSet* params){
    params->cannyL = 150;
    params->cannyH = 200;
    params->ZerBgrL = 10;
    params->ZerBgrH = 50;
    params->cannyCore = 3;
    params->DrawColor = 255;
    params->neighborSize = 7;
    params->VScale = 4;
    params->HScale = 4;
    params->useMedium = true;
}

void FillParamsToView(ParamSet *params){

    char buffer[24];

    memset(buffer,'\0',24);
    sprintf(buffer,"%d",params->cannyL);
    gtk_entry_set_text(GTK_ENTRY(entry_cannyL), buffer);

    memset(buffer,'\0',24);
    sprintf(buffer,"%d",params->cannyH);
    gtk_entry_set_text(GTK_ENTRY(entry_cannyH), buffer);

    memset(buffer,'\0',24);
    sprintf(buffer,"%d",params->ZerBgrL);
    gtk_entry_set_text(GTK_ENTRY(entry_ZerBgrL), buffer);

    memset(buffer,'\0',24);
    sprintf(buffer,"%d",params->ZerBgrH);
    gtk_entry_set_text(GTK_ENTRY(entry_ZerBgrH), buffer);

    memset(buffer,'\0',24);
    sprintf(buffer,"%d",params->cannyCore);
    gtk_entry_set_text(GTK_ENTRY(entry_cannyCore), buffer);

    memset(buffer,'\0',24);
    sprintf(buffer,"%d",params->neighborSize);
    gtk_entry_set_text(GTK_ENTRY(entry_neighborSize), buffer);

    memset(buffer,'\0',24);
    sprintf(buffer,"%d",params->DrawColor);
    gtk_entry_set_text(GTK_ENTRY(entry_drawColor), buffer);

    memset(buffer,'\0',24);
    sprintf(buffer,"%d",params->VScale);
    gtk_entry_set_text(GTK_ENTRY(entry_VScale), buffer);

    memset(buffer,'\0',24);
    sprintf(buffer,"%d",params->HScale);
    gtk_entry_set_text(GTK_ENTRY(entry_HScale), buffer);
}

void SetParamsFromView(ParamSet* params){

    string cannyL_str =  gtk_entry_get_text(GTK_ENTRY(entry_cannyL));
    string cannyH_str =  gtk_entry_get_text(GTK_ENTRY(entry_cannyH));
    string ZerBgrL_str = gtk_entry_get_text(GTK_ENTRY(entry_ZerBgrL));
    string ZerBgrH_str = gtk_entry_get_text(GTK_ENTRY(entry_ZerBgrH));
    string cannyCore_str = gtk_entry_get_text(GTK_ENTRY(entry_cannyCore));
    string neighborSize_str = gtk_entry_get_text(GTK_ENTRY(entry_neighborSize));
    string drawColor_str = gtk_entry_get_text(GTK_ENTRY(entry_drawColor));
    string vscale_str = gtk_entry_get_text(GTK_ENTRY(entry_VScale));
    string hscale_str = gtk_entry_get_text(GTK_ENTRY(entry_HScale));

    params->cannyL = atoi(cannyL_str.c_str());
    params->cannyH = atoi(cannyH_str.c_str());
    params->ZerBgrL = atoi(ZerBgrL_str.c_str());
    params->ZerBgrH = atoi(ZerBgrH_str.c_str());
    params->cannyCore = atoi(cannyCore_str.c_str());
    params->neighborSize = atoi(neighborSize_str.c_str());
    params->VScale = atoi(vscale_str.c_str());
    params->HScale = atoi(hscale_str.c_str());

    int drawColor = atoi(drawColor_str.c_str());
    if(drawColor>255)
        drawColor = 255;
    if(drawColor<0)
        drawColor = 0;

    params->DrawColor = drawColor;

    //setting of using medium filter is in on_checkBox_media_toggle
}


void on_btn_apply_clicked(GtkWidget* button,gpointer userdata){
    SetParamsFromView(_params);
    //gtk_widget_hide(windowParam);
}

void on_btn_cancel_clicked(GtkWidget* button,gpointer userdata){
    gtk_widget_hide(windowParam);
    return;
}

void on_btn_default_clicked(GtkWidget* button,gpointer userdata){

    Use_DefaultParams(_params);
    Fill_DefaultParams();
}

void on_checkBox_meida_toggle(GtkWidget *check_button, gpointer data)
{
    _params->useMedium = !_params->useMedium;
}

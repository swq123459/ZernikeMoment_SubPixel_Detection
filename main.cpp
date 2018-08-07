#include <stdio.h>
#include <pthread.h>
#include <gtk/gtk.h>
#include <opencv2/opencv.hpp>

#include "zernikeMask.h"
#include "subPixelCal.h"

#include "windowParam.h"


using namespace cv;

const char* WndName_CAM = "Video";
const char* WndName_CAP = "Captured";

GThread* Cam_Thread;
VideoCapture cap;

Mat cam_frame;
Mat captured_frame;
GtkWidget *image_captured;
GtkWidget *global_textView;
GtkTextBuffer *global_textBuff;


GtkWidget *ImageSelect;
GtkWidget *entry_pix_mm;
double pixel_mm_ratio = MMPERPIXEL;
#define MMPERPIXEL_STR "0.0145503"

ParamSet params;

bool use_example = false;
volatile static bool keep_Camera_Open=false;

void* CamCtl(void*);

void on_btn_openCam_clicked(GtkWidget *button, gpointer userdata);
void on_btn_closeCam_clicked(GtkWidget *button, gpointer userdata);
void on_btn_capture_clicked(GtkWidget *button, gpointer userdata);

void on_btn_Square_clicked(GtkWidget *button, gpointer userdata);
void on_btn_Circle_clicked(GtkWidget *button, gpointer userdata);

void on_btn_chooseImage_clicked(GtkWidget *button, gpointer userdata);
void on_btn_default_pix_mm_clicked(GtkWidget *button, gpointer userdata);

void on_btn_openParamPanel_clicked(GtkWidget *button, gpointer userData);

void on_checkbox_useExample_toggle(GtkWidget *check_button, gpointer data);

void on_menuitem_params_actived(GtkWidget *widget, GdkEvent *event);


void ShowMessage(const char* message);
void ShowMat(Mat& src);


int main(int argc,char *argv[])
{

    GtkWidget *window;

    GtkWidget *Vlayout_main;
    GtkWidget *Hlayout_up;
    GtkWidget *Vlayout_cameraCtl;
    GtkWidget *Vlayout_processCtl;
    GtkWidget *Hlayout_processCtl_part1;

    GtkWidget *btn_openCam;
    GtkWidget *btn_closeCam;
    GtkWidget *btn_capture;

    GtkWidget *btn_chooseImage;

    GtkWidget *btn_Square;
    GtkWidget *btn_Circle;

    GtkWidget *textView;

    GtkWidget *label_pix_mm;
    GtkWidget *btn_default_pix_mm;
    GtkWidget *btn_openParamPanel;

    GtkWidget *checkbox_useExample;
    checkbox_useExample = gtk_check_button_new_with_label("Use Example");
    g_signal_connect(GTK_OBJECT(checkbox_useExample),"toggled",G_CALLBACK(on_checkbox_useExample_toggle),NULL);


    //menu initial
    GtkWidget *menubar,*menu,*menuitem_setting;
    menubar=gtk_menu_bar_new();

    menuitem_setting = gtk_menu_item_new_with_label("设置");

    gtk_menu_bar_append(GTK_MENU_BAR(menubar),menuitem_setting);
    gtk_widget_show(menuitem_setting);

    menu=gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem_setting),menu);

    GtkWidget *menuitem_params;
    menuitem_params=gtk_menu_item_new_with_label("参数设置");

    gtk_menu_append(GTK_MENU(menu),menuitem_params);
    g_signal_connect(GTK_MENU_ITEM(menuitem_params),"activate",G_CALLBACK(on_menuitem_params_actived),NULL);


    image_captured = gtk_image_new_from_file("./gray.png");


    gtk_init(&argc,&argv);


    //window initial
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window),"Demo");
    gtk_window_set_default_size(GTK_WINDOW(window),640,540);
    gtk_container_set_border_width(GTK_CONTAINER(window),10);

    g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);

    //parameters window initial
    Initial_WindowParam(&params);


    //layout initial
    Vlayout_main = gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(window),Vlayout_main);

    Hlayout_up = gtk_hbox_new(FALSE,0);
    Vlayout_cameraCtl = gtk_vbox_new(FALSE,0);
    Vlayout_processCtl = gtk_vbox_new(FALSE,0);
    Hlayout_processCtl_part1 = gtk_hbox_new(FALSE,0);


    //widgets initial

    textView = gtk_text_view_new();
    global_textView = textView;
    ShowMessage("Camera Closed");

    entry_pix_mm = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_pix_mm),MMPERPIXEL_STR);
    label_pix_mm = gtk_label_new("单位像素对应毫米值");

    btn_openCam = gtk_button_new_with_label("Open Camera");
    g_signal_connect(G_OBJECT(btn_openCam),"clicked",G_CALLBACK(on_btn_openCam_clicked),(gpointer)NULL);

    btn_closeCam = gtk_button_new_with_label("Close Camera");
    g_signal_connect(G_OBJECT(btn_closeCam),"clicked",G_CALLBACK(on_btn_closeCam_clicked),(gpointer)NULL);

    btn_capture = gtk_button_new_with_label("Capture");
    g_signal_connect(G_OBJECT(btn_capture),"clicked",G_CALLBACK(on_btn_capture_clicked),(gpointer)NULL);

    btn_Square = gtk_button_new_with_label("Square-detect");
    g_signal_connect(G_OBJECT(btn_Square),"clicked",G_CALLBACK(on_btn_Square_clicked),(gpointer)NULL);

    btn_Circle = gtk_button_new_with_label("Circle-detect");
    g_signal_connect(G_OBJECT(btn_Circle),"clicked",G_CALLBACK(on_btn_Circle_clicked),(gpointer)NULL);

    btn_chooseImage = gtk_button_new_with_label("本地图片");
    g_signal_connect(G_OBJECT(btn_chooseImage),"clicked",G_CALLBACK(on_btn_chooseImage_clicked),(gpointer)NULL);

    btn_default_pix_mm = gtk_button_new_with_label("default");
    g_signal_connect(G_OBJECT(btn_default_pix_mm),"clicked",G_CALLBACK(on_btn_default_pix_mm_clicked),(gpointer)NULL);

    btn_openParamPanel = gtk_button_new_with_label("参数设置");
    g_signal_connect(G_OBJECT(btn_openParamPanel),"clicked",G_CALLBACK(on_btn_openParamPanel_clicked),(gpointer)NULL);


    //add widgets to layout
    gtk_box_pack_start(GTK_BOX(Vlayout_main),menubar,FALSE,FALSE,3);
    gtk_box_pack_start(GTK_BOX(Vlayout_main),Hlayout_up,FALSE,TRUE,3);
    gtk_box_pack_start(GTK_BOX(Vlayout_main),image_captured,FALSE,TRUE,3);

    gtk_box_pack_start(GTK_BOX(Hlayout_up),Vlayout_cameraCtl,FALSE,FALSE,3);
    gtk_box_pack_start(GTK_BOX(Hlayout_up),Vlayout_processCtl,TRUE,TRUE,3);

    gtk_box_pack_start(GTK_BOX(Vlayout_cameraCtl),btn_openCam,FALSE,TRUE,3);
    gtk_box_pack_start(GTK_BOX(Vlayout_cameraCtl),btn_closeCam,FALSE,TRUE,3);
    gtk_box_pack_start(GTK_BOX(Vlayout_cameraCtl),btn_capture,FALSE,TRUE,3);

    gtk_box_pack_start(GTK_BOX(Vlayout_cameraCtl),btn_Square,FALSE,TRUE,3);
    gtk_box_pack_start(GTK_BOX(Vlayout_cameraCtl),btn_Circle,FALSE,TRUE,3);

    gtk_box_pack_start(GTK_BOX(Vlayout_cameraCtl),checkbox_useExample,FALSE,TRUE,3);


    gtk_box_pack_start(GTK_BOX(Vlayout_processCtl),textView,TRUE,TRUE,3);
    gtk_box_pack_start(GTK_BOX(Vlayout_processCtl),Hlayout_processCtl_part1,FALSE,FALSE,3);
    gtk_box_pack_start(GTK_BOX(Hlayout_processCtl_part1),btn_chooseImage,FALSE,FALSE,3);
    gtk_box_pack_start(GTK_BOX(Hlayout_processCtl_part1),btn_openParamPanel,FALSE,FALSE,3);


    gtk_box_pack_end(GTK_BOX(Hlayout_processCtl_part1),btn_default_pix_mm,FALSE,FALSE,3);
    gtk_box_pack_end(GTK_BOX(Hlayout_processCtl_part1),entry_pix_mm,FALSE,FALSE,3);
    gtk_box_pack_end(GTK_BOX(Hlayout_processCtl_part1),label_pix_mm,FALSE,FALSE,3);



    //show window and launch thread
    gtk_widget_show_all(window);

    gdk_threads_init();

    //Cam_Thread = g_thread_create(CamCtl,NULL,FALSE,NULL);
    Cam_Thread = g_thread_new("Cam_Thread",CamCtl,NULL);

    gdk_threads_enter();
    gtk_main();
    gdk_threads_leave();

    return 0;
}

void ShowMessage(const char* message){
   // GtkTextIter end;
    GtkTextBuffer* buff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(global_textView));
    //gtk_text_buffer_get_end_iter(buff,&end);
    gtk_text_buffer_set_text(buff,message,-1);
//    gtk_text_buffer_insert(buff,&end,"Camera Opened",-1);
}

void ShowMat(Mat& src){
    if(!src.data)
        return;
    Mat scaled_src = src.clone();
    double ratio = 480.0 / (double)src.rows;

    if(ratio > (640.0 / (double)src.cols))
        ratio = 640.0 / (double)src.cols;

    int new_cols = (int) ((double)src.cols * ratio);
    int new_rows = (int) ((double)src.rows * ratio);

    std::cout<<"ratio = "<<ratio<<std::endl;
    std::cout<<"new_cols: "<<new_cols<<" "<<"new_rows: "<<new_rows<<std::endl;

    cv::resize(scaled_src,scaled_src,Size(new_cols,new_rows),0,0);

    imwrite("mat-show.png",scaled_src);
    gtk_image_set_from_file(GTK_IMAGE(image_captured),"mat-show.png");
}

void on_btn_openCam_clicked(GtkWidget *button, gpointer userdata)
{
    cap.release();
    cap = VideoCapture(0);

    namedWindow(WndName_CAM);
    keep_Camera_Open = true;
    usleep(20*1000);//sleep for 30ms
    ShowMessage("Camera Opened");
}

void on_btn_closeCam_clicked(GtkWidget *button, gpointer userdata){
    keep_Camera_Open = false;
    usleep(20*1000);//sleep for 30ms
    destroyWindow(WndName_CAM);
    cap.release();
    ShowMessage("Camera Closed");
}

void on_btn_capture_clicked(GtkWidget *button, gpointer userdata){

    if(!keep_Camera_Open)
    {
        ShowMessage("Please Open Camera");
        return;
    }

    captured_frame = cam_frame.clone();

    if(!captured_frame.data)
        return;

    //imshow(WndName_CAP,captured_frame);
    imwrite("cap.png",captured_frame);
    cv::cvtColor(captured_frame,captured_frame,CV_BGR2GRAY);

    gtk_image_set_from_file(GTK_IMAGE(image_captured),"cap.png");
    ShowMessage("Capture success");

}

void on_checkbox_useExample_toggle(GtkWidget *check_button, gpointer data){
    if(use_example)
        use_example = false;
    else
        use_example = true;
}

void OpenImage(GtkWidget *widget,gpointer *data){
    string image_path = gtk_file_selection_get_filename(GTK_FILE_SELECTION(ImageSelect));
    std::cout<<image_path<<std::endl;

    captured_frame = imread(image_path.c_str(),0);
    if(!captured_frame.data)
    {
        gtk_widget_destroy(GTK_WIDGET(ImageSelect));
        ShowMessage("Error occured when choose Image,\nplease choose again");
        return;
    }

    //gtk_image_set_from_file(GTK_IMAGE(image_captured),image_path.c_str());
    ShowMat(captured_frame);
    gtk_widget_destroy(GTK_WIDGET(ImageSelect));

}

void OpenImage_Cancel(GtkWidget *widget,gpointer *data){
    gtk_widget_destroy(GTK_WIDGET(ImageSelect));
}

void on_btn_chooseImage_clicked(GtkWidget *button, gpointer userdata){
    ImageSelect = gtk_file_selection_new("选择图片");
    gtk_file_selection_set_filename(GTK_FILE_SELECTION(ImageSelect),"*");
    gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(ImageSelect)->ok_button),"clicked",GTK_SIGNAL_FUNC(OpenImage),NULL);
    gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(ImageSelect)->cancel_button),"clicked",GTK_SIGNAL_FUNC(OpenImage_Cancel),NULL);
    gtk_widget_show(ImageSelect);
}

void on_btn_default_pix_mm_clicked(GtkWidget *button, gpointer userdata){
    gtk_entry_set_text(GTK_ENTRY(entry_pix_mm),MMPERPIXEL_STR);
    pixel_mm_ratio = MMPERPIXEL;
}

void on_btn_openParamPanel_clicked(GtkWidget *button, gpointer userData){
    Show_WindowParam();
}

void on_btn_Square_clicked(GtkWidget *button, gpointer userdata){

    ShowMessage("矩形对边距离测量开始\n请先用鼠标框选图中两条矩形的平行边,再按下Esc按键");
    Mat matRoi;
    ZerResult result;
    char disp_buf[1024]={'\0'};

    string ratio = gtk_entry_get_text(GTK_ENTRY(entry_pix_mm));
    double tmp = atof(ratio.c_str());
    if(tmp!=0 && tmp!=HUGE_VAL && tmp!=HUGE_VALF)
        pixel_mm_ratio = tmp;

    if(use_example){
        matRoi = imread("sampleSquare.bmp", 0);
        if(!matRoi.data)
        {
            ShowMessage("Failed to open \"sampleSquare.bmp\"");
            return;
        }
    }
    else{
        matRoi = captured_frame.clone();
        if(!matRoi.data)
        {
            ShowMessage("Please capture a new picture");
            return;
        }

    }

    Point2i p = Point2i(params.VScale,params.HScale);
    result = CalDistanceSquare(matRoi,pixel_mm_ratio,params.DrawColor,params.useMedium,params.cannyL,params.cannyH,params.cannyCore,\
                               params.neighborSize,p,params.ZerBgrL,params.ZerBgrH);

    sprintf(disp_buf,"测量结果:\n对边距离-像素: %lf pix\n对边距离-毫米: %lf mm",\
            result.distance_pixel,result.distance_mm
            );

    ShowMessage(disp_buf);
    ShowMat(result.matedge);

    destroyAllWindows();
}

void on_btn_Circle_clicked(GtkWidget *button, gpointer userdata)
{
    ShowMessage("圆尺寸测量开始\n请先用鼠标框选图中待测量的圆,再按下Esc按键");
    Mat matRoi;
    ZerResultCir resultCir;
    char disp_buf[1024] = {'\0'};

    string ratio = gtk_entry_get_text(GTK_ENTRY(entry_pix_mm));
    double tmp = atof(ratio.c_str());
    if(tmp!=0 && tmp!=HUGE_VAL && tmp!=HUGE_VALF)
        pixel_mm_ratio = tmp;

    if(use_example){
        matRoi = imread("sampleCircle.bmp", 0);
        if(!matRoi.data)
        {
            ShowMessage("Failed to open \"sampleCircle.bmp\"");
            return;
        }
    }
    else{
        matRoi = captured_frame.clone();
        if(!matRoi.data)
        {
            ShowMessage("Please capture a new picture");
            return;
        }

    }
    Point2i p = Point2i(params.VScale,params.HScale);
    resultCir = CalDistanceCircle(matRoi,pixel_mm_ratio,params.DrawColor,params.useMedium,params.cannyL,params.cannyH,params.cannyCore,\
                                  params.neighborSize,p,params.ZerBgrL,params.ZerBgrH);

    sprintf(disp_buf,"测量结果:\n半径: %lf [pixel]\n半径: %lf [mm]\n\n圆心: (%lf , %lf) [pixel]\n圆心: (%lf , %lf)[mm]",\
            resultCir.radius_pixel,resultCir.radius_mm,\
            resultCir.centerPoint_pixel.x,resultCir.centerPoint_pixel.y,\
            resultCir.centerPoint_mm.x,resultCir.centerPoint_mm.y
            );

    ShowMessage(disp_buf);
    ShowMat(resultCir.matedge);

    destroyAllWindows();
}

void on_menuitem_params_actived(GtkWidget *widget, GdkEvent *event)
{
    Show_WindowParam();
}

void* CamCtl(void*){

    while(true){
        if(!keep_Camera_Open)
            usleep(10000);

        while(keep_Camera_Open)
        {
            gdk_threads_enter();
            cap >> cam_frame;
            if(cam_frame.data)
                imshow(WndName_CAM,cam_frame);
            gdk_threads_leave();
            usleep(30000);
        }
    }


    cap.release();
}

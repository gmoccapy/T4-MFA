//
//Türen und Klappen zählen wie folgt: Fahrertür = 1; Beifahrertür = 2; Schiebetür = 4; Heckklappe = 8; Motorhaube = 16
//Fahrertür und Heckklappe offen -> 1 + 8 = 9 --> door_9
//Fahrertür, Beifahrertür und Schiebetür offen-> 1 + 2 + 4 = 7 
//Motorhaube + Schiebetür offen -> 16 + 4 = 20 --> door_20

void door_0(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(36, 77, d_l_c, 15, 54,TFT_WHITE);
  d_spr.drawXBitmap(116, 77, d_r_c, 17, 54,TFT_WHITE);
  d_spr.drawXBitmap(112, 130, d_sd_c, 14, 56,TFT_WHITE);
  d_spr.drawXBitmap(45, 246, d_t_c, 79, 13,TFT_WHITE);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_1(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(6, 77, d_l_o, 39, 44,TFT_RED);
  d_spr.drawXBitmap(116, 77, d_r_c, 17, 54,TFT_WHITE);
  d_spr.drawXBitmap(112, 130, d_sd_c, 14, 56,TFT_WHITE);
  d_spr.drawXBitmap(45, 246, d_t_c, 79, 13,TFT_WHITE);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_2(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(36, 77, d_l_c, 15, 54,TFT_WHITE);
  d_spr.drawXBitmap(124, 77, d_r_o, 37, 46,TFT_RED);
  d_spr.drawXBitmap(112, 130, d_sd_c, 14, 56,TFT_WHITE);
  d_spr.drawXBitmap(45, 246, d_t_c, 79, 13,TFT_WHITE);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_3(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(6, 77, d_l_o, 39, 44,TFT_RED);
  d_spr.drawXBitmap(124, 77, d_r_o, 37, 46,TFT_RED);
  d_spr.drawXBitmap(112, 130, d_sd_c, 14, 56,TFT_WHITE);
  d_spr.drawXBitmap(45, 246, d_t_c, 79, 13,TFT_WHITE);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_4(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(36, 77, d_l_c, 15, 54,TFT_WHITE);
  d_spr.drawXBitmap(116, 77, d_r_c, 17, 54,TFT_WHITE);
  d_spr.drawXBitmap(117, 180, d_sd_o, 15, 56,TFT_RED);
  d_spr.drawXBitmap(45, 246, d_t_c, 79, 13,TFT_WHITE);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_5(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(6, 77, d_l_o, 39, 44,TFT_RED);
  d_spr.drawXBitmap(116, 77, d_r_c, 17, 54,TFT_WHITE);
  d_spr.drawXBitmap(117, 180, d_sd_o, 15, 56,TFT_RED);
  d_spr.drawXBitmap(45, 246, d_t_c, 79, 13,TFT_WHITE);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_6(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(36, 77, d_l_c, 15, 54,TFT_WHITE);
  d_spr.drawXBitmap(124, 77, d_r_o, 37, 46,TFT_RED);
  d_spr.drawXBitmap(117, 180, d_sd_o, 15, 56,TFT_RED);
  d_spr.drawXBitmap(45, 246, d_t_c, 79, 13,TFT_WHITE);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_7(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(6, 77, d_l_o, 39, 44,TFT_RED);
  d_spr.drawXBitmap(124, 77, d_r_o, 37, 46,TFT_RED);
  d_spr.drawXBitmap(117, 180, d_sd_o, 15, 56,TFT_RED);
  d_spr.drawXBitmap(45, 246, d_t_c, 79, 13,TFT_WHITE);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_8(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(36, 77, d_l_c, 15, 54,TFT_WHITE);
  d_spr.drawXBitmap(116, 77, d_r_c, 17, 54,TFT_WHITE);
  d_spr.drawXBitmap(112, 130, d_sd_c, 14, 56,TFT_WHITE);
  d_spr.drawXBitmap(46, 245, d_t_o, 78, 66,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_9(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(6, 77, d_l_o, 39, 44,TFT_RED);
  d_spr.drawXBitmap(116, 77, d_r_c, 17, 54,TFT_WHITE);
  d_spr.drawXBitmap(112, 130, d_sd_c, 14, 56,TFT_WHITE);
  d_spr.drawXBitmap(46, 245, d_t_o, 78, 66,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_10(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(36, 77, d_l_c, 15, 54,TFT_WHITE);
  d_spr.drawXBitmap(124, 77, d_r_o, 37, 46,TFT_RED);
  d_spr.drawXBitmap(112, 130, d_sd_c, 14, 56,TFT_WHITE);
  d_spr.drawXBitmap(46, 245, d_t_o, 78, 66,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_11(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(6, 77, d_l_o, 39, 44,TFT_RED);
  d_spr.drawXBitmap(124, 77, d_r_o, 37, 46,TFT_RED);
  d_spr.drawXBitmap(112, 130, d_sd_c, 14, 56,TFT_WHITE);
  d_spr.drawXBitmap(46, 245, d_t_o, 78, 66,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_12(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(36, 77, d_l_c, 15, 54,TFT_WHITE);
  d_spr.drawXBitmap(116, 77, d_r_c, 17, 54,TFT_WHITE);
  d_spr.drawXBitmap(117, 180, d_sd_o, 15, 56,TFT_RED);
  d_spr.drawXBitmap(46, 245, d_t_o, 78, 66,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_13(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(6, 77, d_l_o, 39, 44,TFT_RED);
  d_spr.drawXBitmap(116, 77, d_r_c, 17, 54,TFT_WHITE);
  d_spr.drawXBitmap(117, 180, d_sd_o, 15, 56,TFT_RED);
  d_spr.drawXBitmap(46, 245, d_t_o, 78, 66,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_14(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(36, 77, d_l_c, 15, 54,TFT_WHITE);
  d_spr.drawXBitmap(124, 77, d_r_o, 37, 46,TFT_RED);
  d_spr.drawXBitmap(117, 180, d_sd_o, 15, 56,TFT_RED);
  d_spr.drawXBitmap(46, 245, d_t_o, 78, 66,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_15(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(6, 77, d_l_o, 39, 44,TFT_RED);
  d_spr.drawXBitmap(124, 77, d_r_o, 37, 46,TFT_RED);
  d_spr.drawXBitmap(117, 180, d_sd_o, 15, 56,TFT_RED);
  d_spr.drawXBitmap(46, 245, d_t_o, 78, 66,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_16(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(36, 77, d_l_c, 15, 54,TFT_WHITE);
  d_spr.drawXBitmap(116, 77, d_r_c, 17, 54,TFT_WHITE);
  d_spr.drawXBitmap(112, 130, d_sd_c, 14, 56,TFT_WHITE);
  d_spr.drawXBitmap(45, 246, d_t_c, 79, 13,TFT_WHITE);
  d_spr.drawXBitmap(46, 38, d_h_o, 77, 35,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_17(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(6, 77, d_l_o, 39, 44,TFT_RED);
  d_spr.drawXBitmap(116, 77, d_r_c, 17, 54,TFT_WHITE);
  d_spr.drawXBitmap(112, 130, d_sd_c, 14, 56,TFT_WHITE);
  d_spr.drawXBitmap(45, 246, d_t_c, 79, 13,TFT_WHITE);
  d_spr.drawXBitmap(46, 38, d_h_o, 77, 35,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_18(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(36, 77, d_l_c, 15, 54,TFT_WHITE);
  d_spr.drawXBitmap(124, 77, d_r_o, 37, 46,TFT_RED);
  d_spr.drawXBitmap(112, 130, d_sd_c, 14, 56,TFT_WHITE);
  d_spr.drawXBitmap(45, 246, d_t_c, 79, 13,TFT_WHITE);
  d_spr.drawXBitmap(46, 38, d_h_o, 77, 35,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_19(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(6, 77, d_l_o, 39, 44,TFT_RED);
  d_spr.drawXBitmap(124, 77, d_r_o, 37, 46,TFT_RED);
  d_spr.drawXBitmap(112, 130, d_sd_c, 14, 56,TFT_WHITE);
  d_spr.drawXBitmap(45, 246, d_t_c, 79, 13,TFT_WHITE);
  d_spr.drawXBitmap(46, 38, d_h_o, 77, 35,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_20(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(36, 77, d_l_c, 15, 54,TFT_WHITE);
  d_spr.drawXBitmap(116, 77, d_r_c, 17, 54,TFT_WHITE);
  d_spr.drawXBitmap(117, 180, d_sd_o, 15, 56,TFT_RED);
  d_spr.drawXBitmap(45, 246, d_t_c, 79, 13,TFT_WHITE);
  d_spr.drawXBitmap(46, 38, d_h_o, 77, 35,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_21(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(6, 77, d_l_o, 39, 44,TFT_RED);
  d_spr.drawXBitmap(116, 77, d_r_c, 17, 54,TFT_WHITE);
  d_spr.drawXBitmap(117, 180, d_sd_o, 15, 56,TFT_RED);
  d_spr.drawXBitmap(45, 246, d_t_c, 79, 13,TFT_WHITE);
  d_spr.drawXBitmap(46, 38, d_h_o, 77, 35,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_22(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(36, 77, d_l_c, 15, 54,TFT_WHITE);
  d_spr.drawXBitmap(124, 77, d_r_o, 37, 46,TFT_RED);
  d_spr.drawXBitmap(117, 180, d_sd_o, 15, 56,TFT_RED);
  d_spr.drawXBitmap(45, 246, d_t_c, 79, 13,TFT_WHITE);
  d_spr.drawXBitmap(46, 38, d_h_o, 77, 35,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_23(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(6, 77, d_l_o, 39, 44,TFT_RED);
  d_spr.drawXBitmap(124, 77, d_r_o, 37, 46,TFT_RED);
  d_spr.drawXBitmap(117, 180, d_sd_o, 15, 56,TFT_RED);
  d_spr.drawXBitmap(45, 246, d_t_c, 79, 13,TFT_WHITE);
  d_spr.drawXBitmap(46, 38, d_h_o, 77, 35,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_24(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(36, 77, d_l_c, 15, 54,TFT_WHITE);
  d_spr.drawXBitmap(116, 77, d_r_c, 17, 54,TFT_WHITE);
  d_spr.drawXBitmap(112, 130, d_sd_c, 14, 56,TFT_WHITE);
  d_spr.drawXBitmap(46, 245, d_t_o, 78, 66,TFT_RED);
  d_spr.drawXBitmap(46, 38, d_h_o, 77, 35,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_25(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(6, 77, d_l_o, 39, 44,TFT_RED);
  d_spr.drawXBitmap(116, 77, d_r_c, 17, 54,TFT_WHITE);
  d_spr.drawXBitmap(112, 130, d_sd_c, 14, 56,TFT_WHITE);
  d_spr.drawXBitmap(46, 245, d_t_o, 78, 66,TFT_RED);
  d_spr.drawXBitmap(46, 38, d_h_o, 77, 35,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_26(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(36, 77, d_l_c, 15, 54,TFT_WHITE);
  d_spr.drawXBitmap(124, 77, d_r_o, 37, 46,TFT_RED);
  d_spr.drawXBitmap(112, 130, d_sd_c, 14, 56,TFT_WHITE);
  d_spr.drawXBitmap(46, 245, d_t_o, 78, 66,TFT_RED);
  d_spr.drawXBitmap(46, 38, d_h_o, 77, 35,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_27(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(6, 77, d_l_o, 39, 44,TFT_RED);
  d_spr.drawXBitmap(124, 77, d_r_o, 37, 46,TFT_RED);
  d_spr.drawXBitmap(112, 130, d_sd_c, 14, 56,TFT_WHITE);
  d_spr.drawXBitmap(46, 245, d_t_o, 78, 66,TFT_RED);
  d_spr.drawXBitmap(46, 38, d_h_o, 77, 35,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
}; 

void door_28(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(36, 77, d_l_c, 15, 54,TFT_WHITE);
  d_spr.drawXBitmap(116, 77, d_r_c, 17, 54,TFT_WHITE);
  d_spr.drawXBitmap(117, 180, d_sd_o, 15, 56,TFT_RED);
  d_spr.drawXBitmap(46, 245, d_t_o, 78, 66,TFT_RED);
  d_spr.drawXBitmap(46, 38, d_h_o, 77, 35,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_29(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(6, 77, d_l_o, 39, 44,TFT_RED);
  d_spr.drawXBitmap(116, 77, d_r_c, 17, 54,TFT_WHITE);
  d_spr.drawXBitmap(117, 180, d_sd_o, 15, 56,TFT_RED);
  d_spr.drawXBitmap(46, 245, d_t_o, 78, 66,TFT_RED);
  d_spr.drawXBitmap(46, 38, d_h_o, 77, 35,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_30(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(36, 77, d_l_c, 15, 54,TFT_WHITE);
  d_spr.drawXBitmap(124, 77, d_r_o, 37, 46,TFT_RED);
  d_spr.drawXBitmap(117, 180, d_sd_o, 15, 56,TFT_RED);
  d_spr.drawXBitmap(46, 245, d_t_o, 78, 66,TFT_RED);
  d_spr.drawXBitmap(46, 38, d_h_o, 77, 35,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
};  

void door_31(void){
  d_spr.createSprite (170,350);
  d_spr.fillSprite(TFT_BLACK);
  d_spr.drawXBitmap(0, 0, d_T4, 170, 350,TFT_WHITE);
  d_spr.drawXBitmap(6, 77, d_l_o, 39, 44,TFT_RED);
  d_spr.drawXBitmap(124, 77, d_r_o, 37, 46,TFT_RED);
  d_spr.drawXBitmap(117, 180, d_sd_o, 15, 56,TFT_RED);
  d_spr.drawXBitmap(46, 245, d_t_o, 78, 66,TFT_RED);
  d_spr.drawXBitmap(46, 38, d_h_o, 77, 35,TFT_RED);
  d_spr.pushSprite(75,65);
  d_spr.deleteSprite();
}; 


`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC ESLABï¼ˆEmbeded System Labï¼?
// Engineer: Haojun Xia & Xuan Wang
// Create Date: 2019/02/22
// Design Name: RISCV-Pipline CPU
// Module Name: HarzardUnit
// Target Devices: Nexys4
// Tool Versions: Vivado 2017.4.1
// Description: Deal with harzards in pipline
//////////////////////////////////////////////////////////////////////////////////
module HarzardUnit(
    input wire CpuRst, ICacheMiss, DCacheMiss, 
    input wire BranchE, JalrE, JalD, 
    input wire [4:0] Rs1D, Rs2D, Rs1E, Rs2E, RdE, RdM, RdW,
    input wire [1:0] RegReadE,
    input wire [2:0] RegWriteM, RegWriteW,
    input wire MemToRegE,
    output reg StallF, FlushF, StallD, FlushD, StallE, FlushE, StallM, FlushM, StallW, FlushW,
    output reg [1:0] Forward1E, Forward2E
    );
    //Stall and Flush signals generate
    always @ (*)
    begin
        if(CpuRst)begin FlushF <= 1'b1; FlushD <= 1'b1; FlushE <= 1'b1; FlushM <= 1'b1; FlushW <= 1'b1;
                    StallF <= 1'b0; StallD <= 1'b0; StallE <= 1'b0; StallM <= 1'b0; StallW <= 1'b0; end
        else
        begin
            if((RdE == Rs1D || RdE == Rs2D) && MemToRegE)       //Load + use
                begin FlushF <= 1'b0; FlushD <= 1'b0; FlushE <= 1'b0; FlushM <= 1'b0; FlushW <= 1'b0; 
                    StallF <= 1'b1; StallD <= 1'b1; StallE <= 1'b0; StallM <= 1'b0; StallW <= 1'b0; end
            else if(BranchE | JalrE)
                begin FlushF <= 1'b0; FlushD <= 1'b1; FlushE <= 1'b1; FlushM <= 1'b0; FlushW <= 1'b0; 
                    StallF <= 1'b0; StallD <= 1'b0; StallE <= 1'b0; StallM <= 1'b0; StallW <= 1'b0; end
            //ÕâÀï·¸ÁËÒ»¸ö´íÎó£¬Ò»¿ªÊ¼ÒÔÎªÊÇIFºÍID¶Î¼ä¼Ä´æÆ÷ÐèÒªÇå¿Õ£¬ºóÀ´·¢ÏÖ£¬Èç¹ûIF¶ÎµÄclearÐÅºÅÎª1£¬Õâ½«µ¼ÖÂÏÂ¸öÖÜÆÚ¿ªÊ¼Ê±£¬ÐÂµÄPCÎÞ·¨Ð´Èë
            else if(JalD)
                begin FlushF <= 1'b0; FlushD <= 1'b1; FlushE <= 1'b0; FlushM <= 1'b0; FlushW <= 1'b0; 
                    StallF <= 1'b0; StallD <= 1'b0; StallE <= 1'b0; StallM <= 1'b0; StallW <= 1'b0; end 
  
            else begin FlushF <= 1'b0; FlushD <= 1'b0; FlushE <= 1'b0; FlushM <= 1'b0; FlushW <= 1'b0; 
                    StallF <= 1'b0; StallD <= 1'b0; StallE <= 1'b0; StallM <= 1'b0; StallW <= 1'b0; end
        end
    end
    
    //Forward Register Source 1
    always @ (*)
    begin
        if(Rs1E == RdM && Rs1E != 5'd0 && RegReadE[1] == 1'b1 && RegWriteM != 3'd0)
            Forward1E <= 2'b10;                 //EX/MEM.rd = ID/EX.rs1, ²¢ÇÒrs1±»ÓÃµ½£¬²¢ÇÒrdÓÐÉÐÎ´Íê³ÉµÄÐ´Èë
        else if(Rs1E == RdW && Rs1E != 5'd0 && RegReadE[1] == 1'b1 && RegWriteW != 3'd0)
            Forward1E <= 2'b01;                 //MEM/WB.rd = ID/EX.rs1, ²¢ÇÒrs1±»ÓÃµ½£¬²¢ÇÒrdÓÐÉÐÎ´Íê³ÉµÄÐ´Èë
        else Forward1E <= 2'b00;
        
        if(Rs2E == RdM && Rs2E != 5'd0 && RegReadE[0] == 1'b1 && RegWriteM != 3'd0)
            Forward2E <= 2'b10;                 //EX/MEM.rd = ID/EX.rs2, ²¢ÇÒrs2±»ÓÃµ½£¬²¢ÇÒrdÓÐÉÐÎ´Íê³ÉµÄÐ´Èë
        else if(Rs2E == RdW && Rs2E != 5'd0 && RegReadE[0] == 1'b1 && RegWriteW != 3'd0)
            Forward2E <= 2'b01;                 //MEM/WB.rd = ID/EX.rs2, ²¢ÇÒrs2±»ÓÃµ½£¬²¢ÇÒrdÓÐÉÐÎ´Íê³ÉµÄÐ´Èë                
        else Forward2E <= 2'b00;
    end
    //Forward Register Source 2

endmodule

//åŠŸèƒ½è¯´æ˜Ž
    //HarzardUnitç”¨æ¥å¤„ç†æµæ°´çº¿å†²çªï¼Œé€šè¿‡æ’å…¥æ°”æ³¡ï¼Œforwardä»¥åŠå†²åˆ·æµæ°´æ®µè§£å†³æ•°æ®ç›¸å…³å’ŒæŽ§åˆ¶ç›¸å…³ï¼Œç»„åˆé?»è¾‘ç”µè·¯
    //å¯ä»¥æœ?åŽå®žçŽ°ã?‚å‰æœŸæµ‹è¯•CPUæ­£ç¡®æ€§æ—¶ï¼Œå¯ä»¥åœ¨æ¯ä¸¤æ¡æŒ‡ä»¤é—´æ’å…¥å››æ¡ç©ºæŒ‡ä»¤ï¼Œç„¶åŽç›´æŽ¥æŠŠæœ¬æ¨¡å—è¾“å‡ºå®šä¸ºï¼Œä¸forwardï¼Œä¸stallï¼Œä¸flush 
//è¾“å…¥
    //CpuRst                                    å¤–éƒ¨ä¿¡å·ï¼Œç”¨æ¥åˆå§‹åŒ–CPUï¼Œå½“CpuRst==1æ—¶CPUå…¨å±€å¤ä½æ¸…é›¶ï¼ˆæ‰€æœ‰æ®µå¯„å­˜å™¨flushï¼‰ï¼ŒCpu_Rst==0æ—¶cpuå¼?å§‹æ‰§è¡ŒæŒ‡ä»?
    //ICacheMiss, DCacheMiss                    ä¸ºåŽç»­å®žéªŒé¢„ç•™ä¿¡å·ï¼Œæš‚æ—¶å¯ä»¥æ— è§†ï¼Œç”¨æ¥å¤„ç†cache miss
    //BranchE, JalrE, JalD                      ç”¨æ¥å¤„ç†æŽ§åˆ¶ç›¸å…³
    //Rs1D, Rs2D, Rs1E, Rs2E, RdE, RdM, RdW     ç”¨æ¥å¤„ç†æ•°æ®ç›¸å…³ï¼Œåˆ†åˆ«è¡¨ç¤ºæºå¯„å­˜å™?1å·ç ï¼Œæºå¯„å­˜å™?2å·ç ï¼Œç›®æ ‡å¯„å­˜å™¨å·ç 
    //RegReadE RegReadD[1]==1                   è¡¨ç¤ºA1å¯¹åº”çš„å¯„å­˜å™¨å€¼è¢«ä½¿ç”¨åˆ°äº†ï¼ŒRegReadD[0]==1è¡¨ç¤ºA2å¯¹åº”çš„å¯„å­˜å™¨å€¼è¢«ä½¿ç”¨åˆ°äº†ï¼Œç”¨äºŽforwardçš„å¤„ç?
    //RegWriteM, RegWriteW                      ç”¨æ¥å¤„ç†æ•°æ®ç›¸å…³ï¼ŒRegWrite!=3'b0è¯´æ˜Žå¯¹ç›®æ ‡å¯„å­˜å™¨æœ‰å†™å…¥æ“ä½?
    //MemToRegE                                 è¡¨ç¤ºExæ®µå½“å‰æŒ‡ä»? ä»ŽData Memoryä¸­åŠ è½½æ•°æ®åˆ°å¯„å­˜å™¨ä¸­
//è¾“å‡º
    //StallF, FlushF, StallD, FlushD, StallE, FlushE, StallM, FlushM, StallW, FlushW    æŽ§åˆ¶äº”ä¸ªæ®µå¯„å­˜å™¨è¿›è¡Œstallï¼ˆç»´æŒçŠ¶æ€ä¸å˜ï¼‰å’Œflushï¼ˆæ¸…é›¶ï¼‰
    //Forward1E, Forward2E                                                              æŽ§åˆ¶forward
//å®žéªŒè¦æ±‚  
    //å®žçŽ°HarzardUnitæ¨¡å—   
			AREA    | .text|, CODE, READONLY, ALIGN=2
	        THUMB
	        EXTERN  currentPt
	        EXPORT	SysTick_Handler
	        EXPORT  osSchedulerLaunch
		



SysTick_Handler                           ;save R0,R1,R2,R3,R12,LR,PC,PSR
          CPSID      I                        ;disable interrupts
	      PUSH      {R4-R11}                  ;save R4,R5,R6,R7,R8,R9,R10,R11            
	      LDR       R0, =currentPt            ;R0 points to currentPt
	      LDR       R1,[R0]                   ;R1 = currentPt
	      STR       SP,[R1]
	      LDR       R1,[R1,#4]                ;R1 = currentPt->next
	      STR       R1,[R0]                   ;currentPt = R1
	      LDR       SP,[R1]                   ;SP = currentPt->stackPt
	      POP       {R4-R11}
	      CPSIE      I
	      BX        LR



osSchedulerLaunch                         ;loading the SP by the content of the 1st thread
          LDR       R0, =currentPt
	      LDR       R2,[R0]                   ;R2 = currentPt
	      LDR       SP,[R2]                   ;SP = currentPt->stackPt
	      POP       {R4-R11}
	      POP       {R0-R3}
		  pop       {R12}
	      ADD       SP,SP,#4
	      POP       {LR}
	      ADD       SP,SP,#4
	      CPSIE      I
	      BX        LR
	  
	  
	  
   ALIGN
   END
	  
	  
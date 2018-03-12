#include "regret.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void min_loss(log_type log, double *sum_loss){
	int y[O_NUM] = {0}, type;
	double x[W_NUM], wx_best[O_NUM], loss[log.count], loss_temp;
	double w_best[O_NUM][W_NUM], d_loss[O_NUM][W_NUM], d_loss_sum[O_NUM][W_NUM];

	for(int T = 0; T < log.count; T++){ //log.count
		for(int i = 0; i < O_NUM; i++){ //initializa weight
    		for(int j = 0; j < W_NUM; j++){
    			w_best[i][j] = 0.0;
    		}
    	}
    	loss_temp = 999.9;    	

    	for(int n = 0; n < LOOP_NUM; n++){
    	// do {

    		loss[T] = loss_temp;
			loss_temp = 0.0;
    		for(int i = 0; i < O_NUM; i++){ //initializa weight
    			for(int j = 0; j < W_NUM; j++){
    				d_loss_sum[i][j] = 0.0;
    			}
    		}    		
    		
    		for(int t = 0; t < T+1; t++){
    			assign_output(log, T, y, &type);
    			assign_input(log, T, x);						
    			multiply_vectors(wx_best, &w_best, x);
    			loss_temp = loss_temp + loss_calc(&w_best, x, y); //sum of loss of each (xi, yi)    			
    			update_gradient(&d_loss, wx_best, x, y);
    			for(int i = 0; i < O_NUM; i++){ //initializa weight
    				for(int j = 0; j < W_NUM; j++){
    					d_loss_sum[i][j] = d_loss_sum[i][j] + d_loss[i][j];
    				}
    			} 			  			
			}
			update_weight(&d_loss, &w_best, ALPHA); 

			// printf("Loss_%d %.4f %.4f %.4f\n", T, loss[T], loss_temp, loss[T] - loss_temp);
		}										
		// }while(fabs(loss[T] - loss_temp) > ERR);
        printf("%d %.4f \n", T, loss[T]);
		// printf("Loss_%d %.4f %.4f %.4f\n", T, loss[T], loss_temp, loss[T] - loss_temp);
		// if(T == 0){
		// 	sum_loss[T] = loss[T];
		// }else{
		// 	sum_loss[T] = sum_loss[T-1] + loss[T];
		// }
		// printf("Loss_%d %.4f %.4f\n", T, loss[T], sum_loss[T]);
	}
}
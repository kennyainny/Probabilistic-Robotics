% clean up the matlab environment
clear; clc; close all;

% run initialization of some paths and variables
init_setup;
load('lab3.mat');
% contains A, B, C, LQR_Kss, target_hover_state, clipping_distance
% C = eye(size(x)) %full-state

H = 2000;

% actual state, not observable, do not reference
x(:,1) = target_hover_state;

% initial state estimate (given)
mu_x(:,1) = x(:,1);
% initial state observation (given)
y(:,1) = x(:,1);

% initial control
dx = compute_dx(target_hover_state, mu_x(:,1));
u(:,1) = LQR_Kss* dx;

% noise parameters X -> Motion model, Y -> Observation
sigmaY = 0.5; %default 0.5; 
%fail at 0.4, 0.7, 0.8 for no filter, KF, EKF, respectively

sigmaX = 0.1; %default 0.1; 
%fail at 3.0, 3.5, 3.8 for no filter, KF, EKF, respectively

% Define Parameters
P = 1*eye(size(x,1)); %initial covariance matrix
Q = 2*eye(size(x,1)); %process noise matrix
R = 50*eye(size(x,1)); %observation noise matrix

for t=2:H    
	% add noise to motion model:
    noise_F_T = randn(6,1)*sigmaX;
    
    % Simulate helicopter, do not change:
    x(:,t) = f_heli(x(:,t-1), u(:,t-1), dt, model, idx, noise_F_T);
    
    % add state observation noise
    v = randn(size(C*x(:,t)))*sigmaY;
    
    % observe noisy state
    y(:,t) = C*x(:,t) + v;    
    
    %%%%%%%%%%%%%%%%%%%% Implementing Kalman Filtering %%%%%%%%%%%%%%%%%%%%
    
    % use Kalman filter to calculate mean state
    % from mu_x(:,t-1), y(t) ,u(t-1)       
    abcdekfghij = 2; % 0=No Filter, 1=KF, 2=EKF    
    
    if abcdekfghij == 0
      %%%%%%%%%% No Filter %%%%%%%%%% 
        mu_x(:,t) = y(:,t); % dumb take observation as estimate
    
    elseif abcdekfghij == 1
      %%%%%%%%%% KF %%%%%%%%%%    
        % Time Update
        mu_x_bar(:,t) = A*mu_x(:,t-1) + B*u(:,t-1);
        P_bar = A*P*A' + Q;
    
        K = P_bar*C'/(C*P_bar*C' + R); % Kalman Gain
        e = y(:,t) - C*mu_x_bar(:,t); % innovation
    
        % Measurement Update
        mu_x(:,t) = mu_x_bar(:,t) + K*e;
        P = (eye(size(x,1)) - K*C)*P_bar;
    
    elseif abcdekfghij == 2
      %%%%%%%%%% EKF %%%%%%%%%%
        % Time Update
        mu_x_bar(:,t) = f_heli(mu_x(:,t-1), u(:,t-1), dt, model, idx, zeros(6,1)); %E(noise) = 0
        [A_EKF, ~] = linearized_dynamics(mu_x(:,t-1), u(:,t-1), mu_x(:,t-1), mu_x_bar(:,t), @f_heli, dt, model, idx, zeros(6,1), 0, 0);
        P_bar = A_EKF*P*A_EKF' + Q;
        
        % Since it is a full-state feedback, g(x) = x -> C = I
        C_EKF = eye(size(x,1));
        K = P_bar*C_EKF'/(C_EKF*P_bar*C_EKF' + R); % Kalman Gain
        e = y(:,t) - mu_x_bar(:,t); % innovation
    
        % Measurement Update
        mu_x(:,t) = mu_x_bar(:,t) + K*e;
        P = (eye(size(x,1)) - K*C_EKF)*P_bar;
    end
    
    %%%%%%%%%%%%%%%% End of Implementing Kalman Filtering %%%%%%%%%%%%%%%%%
    
    % LQR controller generates control for next step.  u(t-1) takes x(:,t-1)
    % to x(:,t).  do not change (only change mu_x value above)
	dx = compute_dx(target_hover_state, mu_x(:,t));
    dx(idx.ned) = max(min(dx(idx.ned), clipping_distance),-clipping_distance);
	u(:,t) = LQR_Kss* dx;   
    
end

figure; plot(x(idx.ned,:)'); legend('north', 'east', 'down'); title('Hover position');
figure; plot(x(idx.q,:)'); legend('qx', 'qy', 'qz', 'qw'); title('Hover quaternion');
figure; plot(x(idx.u_prev,:)'); legend('aileron','elevator','rudder','collective'); title('Hover trim');


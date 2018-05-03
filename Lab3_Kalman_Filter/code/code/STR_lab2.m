% clean up the matlab environment
clear; clc; close all;

% run initialization of some paths and variables
init_setup;
load('lab3.mat');
% contains A, B, C, LQR_Kss, target_hover_state, clipping_distance
% C = eye(size(x))

H = 2000;

% actual state, not observable, do not reference
x(:,1) = target_hover_state;

% initial state estimate (given)
mu_x(:,1) = x(:,1);

% initial covariance
covar_t = eye(21); %*0.01; %zeros(21,21);

% initial state observation (given)
y(:,1) = x(:,1);

% initial control
dx = compute_dx(target_hover_state, mu_x(:,1));
u(:,1) = LQR_Kss*dx;

% noise parameters
sigmaX = 10;
sigmaY = 10;

Q = eye(21)*sigmaX*sigmaX; %process noise
R = eye(21)*sigmaY*sigmaY; %measurement noise


for t=2:H    
	% add noise to motion model:
    noise_F_T = randn(6,1)*sigmaX;
    
    % Simulate helicopter, do not change:
    x(:,t) = f_heli(x(:,t-1), u(:,t-1), dt, model, idx, noise_F_T);
    
    % add state observation noise
    v = randn(size(C*x(:,t)))*sigmaY;

    % observe noisy state
    y(:,t) = C*x(:,t) + v;    
    
    % use Kalman filter to calculate mean state
    % from mu_x(:,t-1), y(t) ,u(t-1)
    
%     mu_x(:,t) = y(:,t); % dumb take observation as estimate
    
    %% KF algorithm
    %x(:,t) = f_heli(x(:,t-1), u(:,t-1), dt, model, idx, noise_F_T);
    %[A , B] = linearized_dynamics(x0, u0, xt0, xt1, simulate_f, dt_sim, model, idx, model_bias, magic_factor, x1star)
    %[A , B] = linearized_dynamics(x(:,1), u(:,1), x(:,t-1), x(:,t), @f_heli, dt, model, idx, zeros(6,1), 0, 0);
%       size(A)
%       size(B)
    %Prediction
    mu_x_ = A*mu_x(:,t-1) + B*u(:,t-1); %x_dot = Ax+Bu
    covar_t_ = A*covar_t*A' + Q;
    k_gain = covar_t_*C'/(C*covar_t_*C' + R);
    innov = y(:,t) - C*mu_x_;
    
    %Correction
    mu_x(:,t) = mu_x_ + k_gain*innov;
    covar_t = covar_t_ - k_gain*C*covar_t_;
    
    
    %% Continue
    % LQR controller generates control for next step.  u(t-1) takes x(:,t-1)
    % to x(:,t).  do not change (only change mu_x value above)
	dx = compute_dx(target_hover_state, mu_x(:,t));
    dx(idx.ned) = max(min(dx(idx.ned), clipping_distance),-clipping_distance);
	u(:,t) = LQR_Kss* dx;   
    
%     disp("x_t: ");x(:,t)'
%     disp("y_t: ");y(:,t)'
    
end

figure; plot(x(idx.ned,:)'); legend('north', 'east', 'down'); title('Hover position');
figure; plot(x(idx.q,:)'); legend('qx', 'qy', 'qz', 'qw'); title('Hover quaternion');
figure; plot(x(idx.u_prev,:)'); legend('aileron','elevator','rudder','collective'); title('Hover trim');


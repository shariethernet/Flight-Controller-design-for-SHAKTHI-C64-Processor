%% startVars.m - Initialize variables
% This script initializes variables and buses required for the model to
% work. Mask block parameters are defined by structures that define the
% location of the block, ie. If the Initial Condition parameter is located
% under Vehicle/Nonlinear/Integrator the variable is set to
% Vehicle.Nonlinear.Integrator.initialCondition = 0;

%   Copyright 2013-2019 The MathWorks, Inc.

% Register variables in the workspace before the project is loaded
initVars = who;

% Variants Conditions
asbVariantDefinition;
VSS_COMMAND = 0;       % 0: Signal Editor, 1: Joystick, 2: Pre-saved data, 3: Pre-saved data in a Spreadsheet
VSS_SENSORS = 1;       % 0: Feedthrough, 1: Dynamics
VSS_VEHICLE = 1;       % 0: Linear Airframe, 1: Nonlinear Airframe.
VSS_ENVIRONMENT = 0;   % 0: Constant, 1: Variable
VSS_ACTUATORS = 0;     % 0: Feedthrough, 1: Linear Second Order, 2: Noninear Second Order

% Enable SL3D visualization variant only if a license for SL3D is available
if license('test','Virtual_Reality_Toolbox')
    VSS_VISUALIZATION = 3; % 0: Scopes, 1: Send values to workspace, 2: FlightGear, 3: Simulink 3D.
else
    VSS_VISUALIZATION = 0; % 0: Scopes, 1: Send values to workspace, 2: FlightGear, 3: Simulink 3D.
end

% Bus definitions 
asbBusDefinitionCommand; 
asbBusDefinitionSensors;
asbBusDefinitionEnvironment;
asbBusDefinitionStates;

% Sampling rate
Ts= 0.2;

% Mass properties
mass = 1;
inertia = eye(3);

% Initial contitions
initDate = [2013 1 1 0 0 0];
initPosLLA = [37.628738616666666 -1.223933911333333e+02 100];
initPosNED = [0 0 -100];
initVb = [0 0 0];
initEuler = [0 0 0];
initAngRates = [0 0 0];

%% Custom Variables
% Add your variables here:
% myvariable = 0;

% Register variables after the project is loaded and store the variables in
% initVars so they can be cleared later on the project shutdown.
endVars = who;
initVars = setdiff(endVars,initVars);
clear endVars;

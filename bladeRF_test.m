% We must first tell MATLAB/Octave where to find our functions
% by pointing it to the directory containing the conversion routines.
% You will need to change this path as needed.
addpath('/Users/camilamenard/bladeRF/host/misc/matlab/')

% Load the samples
signal = load_sc16q11('Users/camilamenard/bladeRF/host/libraries/libbladeRF/include/bladeRF_1.csv');

% Plot the magnitude of the signal in the time domain
plot(abs(signal));
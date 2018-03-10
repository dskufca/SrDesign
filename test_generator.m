% Uncomment individual Example lines.

% Modify which satellite number you want to generate for
sat_num = 1

%**** L1CA ****************************************************************
%---- Example 1: LI C/A (nyquist frequency)
modulation = 'L1CA';
fs = 1.023e6;
prn_chips = 1023;
prn_time = 1e-3;
n_periods = 1;

%---- Example 2: LI C/A (higher frequency)
% modulation = 'L1CA';
% fs = 12.7e6;  % arbitrary
% prn_chips = 1023;
% prn_time = 1e-3;
% n_periods = 1;

%---- Example 3: LI C/A (sub-nyquist frequency)
% modulation = 'L2CL';
% fs = 1e6;
% prn_chips = 1023;
% prn_time = 1e-3;
% n_periods = 1;

%---- Example 4: LI C/A (several periods, higher frequency) 
%---- uncomment the last plot to check that the signal is repeated
% modulation = 'L1CA';
% fs = 2.12e6;           % arbitrary
% prn_chips = 1023;
% prn_time = 1e-3;
% n_periods = 15.71;  % arbitrary

%---- Example 5: LI C/A (several periods, sub-nyquist frequency) 
%---- uncomment the last plot to check that the signal is repeated
% modulation = 'L1CA';
% fs = 0.98e6;           % arbitrary
% prn_chips = 1023;
% prn_time = 1e-3;
% n_periods = 15.71;  % arbitrary

%---- Example 6: LI C/A (smaller period)
% modulation = 'L1CA';
% fs = 1.023e6;
% prn_chips = 1023;
% prn_time = 1e-3;
% n_periods = 0.518;  % arbitrary


%*************************************************************************


[I Q] = GNSSsignalgen(sat_num, modulation,fs,n_periods);
signal = I+1j*Q;
n_chips = length(signal)/fs*prn_chips/prn_time;

I = int16(I);
Q = int16(Q);
IQsamps((1:length(I))*2 - 1) = I;
IQsamps((1:length(Q))*2) = Q;


fileID = fopen('/Users/camilamenard/Desktop/samples.dat', 'w');
fwrite(fileID, IQsamps, 'int16');
fclose(fileID);

%==========================================================================  
%% Observables
%==========================================================================  

time_chips = linspace(1,n_chips,length(signal));
time_s = linspace(0,length(signal)/fs,length(signal));
freq = linspace(-fs/2,fs/2,length(signal));

SIGNAL = fft(signal)/sqrt(length(signal));
ACF = fftshift(ifft(SIGNAL.*conj(SIGNAL))); % Auto-correlation

freq_dopp = linspace(-5e3,5e3,301);          % Woodward ambiguity function
WAF = zeros(length(signal),length(freq_dopp));
for i=1:length(freq_dopp)
    fasor = exp(-1j*2*pi*freq_dopp(i).*time_s)';
    WAF(:,i) = fftshift(ifft(fft(fasor.*signal).*conj(SIGNAL)));
end
WAF = WAF/sqrt(length(signal));

%==========================================================================  
%% Plots
%==========================================================================  
%----  Eye diagram (only for complex signals) -----------------------------
figure,
        plot(signal);
        title('Eye diagram');
        xlabel('I');
        ylabel('Q');
        grid on;

%---- Signal Plot  --------------------------------------------------------
figure, 
        subplot(2,1,1);
        stairs(time_s*1e3,real(signal)); xlabel('time (ms)');    % plot time
%         stairs(time_chips,real(signal)); xlabel('time (chips)'); % plot chips            
        title('I');
        ylabel('amplitude');
        grid on;

        subplot(2,1,2); 
        stairs(time_s*1e3,imag(signal)); xlabel('time (ms)');        % plot time
%         stairs(time_chips,imag(signal)); xlabel('time (chips)'); % plot chips
        title('Q');
        ylabel('amplitude');
        grid on;

%---- Auto-correlation plot (absolute value) ------------------------------
figure,
        plot(time_s*1e3,abs(ACF)); xlabel('time (ms)');    % plot time
%         plot(time_chips*1e3,abs(ACF)); xlabel('time (chips)'); % plot chips
        title('ACF');
        ylabel('power');
        grid on;

%---- Spectrum ------------------------------------------------------------
figure,
        plot(freq/1e6,10*log10(abs(fftshift(SIGNAL)).^2))
        title('1-Watt normalized spectrum');
        xlabel('Frequency (MHz)');
        ylabel('power (log)');
        grid on;
        
%---- Woodward ambiguity function -----------------------------------------
figure,
        mesh(freq_dopp/1e3,time_s*1e3,abs(WAF).^2);
        title('WAF');
        xlabel('Doppler (kHz)');
        ylabel('delay (ms)');        

%---- Check signal period -------------------------------------------------
% for i=1:floor(n_periods)-1
%     RES(i) = ~isequal(signal(prn_time*fs*(i-1)+1:prn_time*fs*i),signal(prn_time*fs*i+1:prn_time*fs*(i+1)));
% end
% 
% figure, 
%         plot(1:floor(n_periods)-1,RES);
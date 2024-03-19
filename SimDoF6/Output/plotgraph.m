load Result.txt;
data=Result;

figure(100);
subplot(2,1,1);
plot(data(:,1),data(:,2),data(:,1),data(:,3),data(:,1),data(:,4));
xlabel('time(s)');ylabel('position(m)')

subplot(2,1,2);
plot(data(:,1),data(:,5),data(:,1),data(:,6),data(:,1),data(:,7));
xlabel('time(s)');ylabel('velocity(m/s)')

figure(200);
plot(data(:,1),data(:,12),data(:,1),data(:,13),data(:,1),data(:,14));
xlabel('time(s)');ylabel('angle velocity(m/s)')
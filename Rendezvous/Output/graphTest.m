
h=10
errmax = 0.1
SAFETY=0.9, ERRCON=1.89e-4;
PGROW  = -1/4.0;

errmax = [1:0.001:10];
yy = errmax.^PGROW;


figure(300);
plot(errmax,yy,'-');
xlabel('time(s)');ylabel('position(m)');
legend('x','y','z');


y1=int32(y1);

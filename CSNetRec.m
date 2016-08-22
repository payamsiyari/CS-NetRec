function  [xpp, precision, recall, fmeasure, CSError, prededges, nos] = CSNetRec(logInputMatrix,logy,pathnet,n,numberOfCascades,logging)
%Inputs:
%   logInputMatrix - log(InputMatrix)
%   logy - log(y)
%   pathnet - Path of the network
%   n - number of nodes
%   numberOfCascades - number of cascades
%   logging - option to enable logging

%Outputs:
%   xpp - predicted x
%   precision - precision of predection
%   recall - recall of predection
%   fmeasure - fmeasure of predection
%   CSError - CSError of predection
%   prededges - predicted edges
%   nos - Number of edges seen in the cascades

a=logInputMatrix(1:numberOfCascades,:);
y=logy(1:numberOfCascades,:);
logZeroConst=-10000000;
for i=1:length(y)
    if (y(i,1)==-Inf)
        y(i,1)=logZeroConst;
    end
end
disp('Parsing Network...');
%Reading NET
edges=zeros(n*(n-1),1);
fid2 = fopen(pathnet, 'r');
for x=1:n+1
     fgetl(fid2);
end
while 1
    tline = fgetl(fid2);
    if ~ischar(tline), break, end
    v1=regexp(tline,',','split');
    v2=str2double(v1(1,2));
    v1=str2double(v1(1,1));
    
    edges((v1)*(n-1)+v2,1)=1;            
end
fclose(fid2);
disp('Parsing Network Finished!');
nos=0;%Number of edges seen in the cascades
tmpedges=zeros(n*(n-1),1);
                                                                                       y=a*edges;
    disp('Solving...');
    cvx_clear;
    cvx_precision('high');
    if(logging==false)
        cvx_quiet(true);
    else
        cvx_quiet(false);
    end
    cvx_solver('sdpt3');
    cvx_begin
        variable xp(n*(n-1));
        minimize(norm(xp,1)+norm(a*xp - y,2));
        subject to
            0 <= xp <= 1;
%             a*xp==y;
    cvx_end
    disp('Solving Finished!');

CSError=0;
truep=0;p=0;
xp=sum(a)';
precision=[];recall=[];fmeasure=[];prededges=[];
[xpp ix]=sort(xp,'ascend');
for k=[50:50:nnz(edges)-mod(nnz(edges),50) nnz(edges)]
    p=k;
    truep=0;
    for j=1:k
        if(edges(ix(j,1),1)>0)
            truep=truep+1; 
        end
    end
    precision=[precision truep/p];
    recall=[recall truep/nnz(edges)];
    fmeasure=[fmeasure 2*(truep/p)*(truep/nnz(edges))/((truep/p)+(truep/nnz(edges)))];
    prededges=[prededges p];
end
    disp('Successfully finished!');
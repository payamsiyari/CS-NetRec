%Inputs:
%   path - cascade file path
%   n - number of nodes
%   numberOfCascades - Number of Cascades
%   Alpha - alpha parameter

%Outputs:
%   a - A matrix
%   y - y vector
function[a,y] = extractCascMatrix(path,n,numberOfCascades,Alpha)
fid = fopen(path,'r');

disp('Parsing Cascades...');
a=[];
y=[];
for x=1:n+1
     fgetl(fid);
end
for qwe=1:numberOfCascades
    tline = fgetl(fid);
    tline = fgetl(fid);
    if ~ischar(tline), break, end
    if(findstr(';',tline)) 


        tmp=ones(1,n*(n-1));

        t = regexp(tline,';','split');
        for i=1:length(t)-1
                tmpstr=regexp(t(1,i),',','split');
                
                node1=regexp(tmpstr{1,1}(1,1),'-','split');
                nodeID1=str2double(node1{1,1}(1,1));
                nodeTime1=str2double(node1{1,1}(1,2));
                
                node2=regexp(tmpstr{1,1}(1,2),'-','split');
                nodeID2=str2double(node2{1,1}(1,1));
                nodeTime2=str2double(node2{1,1}(1,2));
                
                tmp(1,(nodeID1)*(n-1)+nodeID2)=Alpha*exp(-1*Alpha*(nodeTime2-nodeTime1));
        end
        a=sparse([a;sparse(log(tmp))]);
        y=[y;str2double(t(1,length(t)))];
    end
    if(mod(qwe,50)==0)
        disp(['Cascade ' num2str(qwe) ' Passed.']);
    end
end
fclose(fid);
end
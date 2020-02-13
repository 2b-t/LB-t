%% github.com/2b-t
%  December 2019

%% Geometry importer
%  Imports the geometry from the 'Domain.txt' file outputted by the C++ 
%  file.
%
%% Arguments
%  skip: optional argument to skip the precise geometry import as it might
%  take long for large three-dimensional domain to load all the indices 
%  from a text file
%
%% Return values
%  BounceBack: logical domain that contains the wall cells
%
function BounceBack = GeometryImport(import)

    % if optional argument was not defined
    if ~exist('import', 'var')
        import = false; %set default value
    end

    %% open file
    fileID = fopen('BIN/Domain.txt'); %open file (relative path)
    
    if (fileID == -1) %if opening file failed
        disp('- File <Domain.txt> not found.')
        return
    end

    %% import matrix dimensions
    fgetl(fileID); %discard second line
    currline = fgetl(fileID); %get second line (dimensions)
    split = strsplit(currline, {', ',';'}, 'CollapseDelimiters', true); %split line according to delimiters
    NX = str2num(cell2mat(split(1))); %extract dimensions
    NY = str2num(cell2mat(split(2)));
    NZ = str2num(cell2mat(split(3)));
    BounceBack = false(NX, NY, NZ); %create bounce-back matrix
    
    fgetl(fileID); %discard third line
    currline = fgetl(fileID); %get fourth line (number of bounce-back cells)
    split = strsplit(currline,'...'); %split line according to delimiters
    BBnumber = str2num(cell2mat(split(1))); %extract number of bounce-back cells
    BBindex = zeros(BBnumber, 1); %pre-allocate memory
    
    %% import bounce-back indices
    n = 4; %number of lines
    while ischar(currline) %identify number of lines
        currline = fgetl(fileID); %get current line
        n = n+1; %keep counting
    end
    
    frewind(fileID); %rewind file
    for i = 1:6 %skip the next two lines as well
        fgetl(fileID);
    end
    
    if (import == true) %if skipping was chosen
        
        j = 1; %counter for BBindex
    
        for i = 7:(n-1) %for all the relevant lines (last one empty)
            currline = fgetl(fileID); %read line
            split = strsplit(currline, {',',';'}, 'CollapseDelimiters', true); %split line according to delimiters
            k = 1; %reset counter
            while ((k <= 3) && (j <= BBnumber)) %copy all values to Matrix as long as necessary
                BBindex(j) = str2num(cell2mat(split(k))); %save to matrix
                j = j+1; %increment
                k = k+1;
            end
        end
     
        %% convert bounce-back indices to coordinates
        for i = 1:length(BBindex)
            z = floor(BBindex(i)/(NX*NY))+1; %find z index through division (account for offset due to matlab matrices start at 1 not 0)
            rest = mod(BBindex(i), NX*NY); %find division rest
            y = floor(rest/NX)+1; %find y index through division (account for offset due to matlab matrices start at 1 not 0)
            x = mod(rest, NX)+1; %find x index through rest (account for offset due to matlab matrices start at 1 not 0)
            BounceBack(x, y, z) = true; %set corresponding bounce-back nodes
        end
        
    end
    
    fclose(fileID); %close file

end
%% github.com/2b-t
%  December 2019

%% ParametersImport
%  Imports initial conditions and characteristic measures such as
%  characteristic length and velocity as well as overall simulation
%  timesteps from a *txt-file of given format.
%
%  arguments:
%  FILENAME: name of the file to be imported
%
%  return values:
%  NX:    resolution in x-direction
%  NY:    resolution in y-direction
%  NZ:    resolution in z-direction
%  NT:    number of overall simulation time steps
%  RE:    Reynolds number
%  RHO_0: initial density
%  L:     characteristic length
%  U:     characteristic velocity
%
function [NX, NY, NZ, NT, RE, RHO_0, L, U] = ParametersImport(FILENAME)

        function parameter = getParameter()
            delimiters = {' ',', ',';'}; %define delimiters
            curr_line = fgetl(fileID); %get new line
            split = strsplit(curr_line, delimiters, 'CollapseDelimiters', true); %split line
            parameter = str2num(cell2mat(split(2))); %extract relevant part
        end
    
    %% Open file
    fileID = fopen(FILENAME); %open file (relative path)

    if fileID == -1 %if opening file failed
        disp(['- File <' FILENAME '> not found.'])
        return
    end
    
    %% Header: discard first ten lines
    for(i = 1:10)
      fgetl(fileID);
    end
    
    %% Get spatial resolution
    NX = getParameter();
    NY = getParameter();
    NZ = getParameter();
    
    %% Get temporal resolution
    currline = fgetl(fileID); %get 14th line
    currline = fgetl(fileID); %get 15th line
    NT = getParameter();
    
    %% Get physical parameters
    currline = fgetl(fileID); %get 17th line
    currline = fgetl(fileID); %get 18th line
    RE    = getParameter();
    RHO_0 = getParameter();
    L     = getParameter();
    U     = getParameter();

    fclose(fileID); %close file
    
end
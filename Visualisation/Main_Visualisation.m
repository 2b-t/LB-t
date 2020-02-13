%% github.com/2b-t
%  December 2019

%% Importer and visualiser
% This tool imports the relevant geometry, density and velocity outputted 
% by the C++ application in form of *.bin-files and is able to visualise 
% it in 2D. 
% The tool searches for new files and stops if no follow-up file is found
% within tmax seconds.
%
% requires: isOctave.m, ParametersImport.m, GeometryImport.m, StepImport.m, 
%           Visualisation.m, Residuals.m
%
function Main_Visualisation

        %% Import numbers
        %  Nested function for importing relevant file name numbers 
        %
        %  arguments:
        %  FILENAME: naming scheme of the export files e.g. "macro_"
        %
        %  return values:
        %  fielno:   all file numbers that correspond to the naming scheme
        %
        function fieldno = ImportNumber(FILENAME)
            files = dir([pwd '/BIN/' FILENAME '*']); %identify relevant files
            [filesno, ~] = size(files); %number of files found
            fieldno = zeros(filesno, 1); %allocate space
            
            for i = 1:filesno %for all files found
                [~, name, ~] = fileparts(files(i).name); %get name of files
                split = strsplit(name,'_'); %split into parts
                nameno = str2num(cell2mat(split(2))); %isolate number
                files(i).number = nameno; %assign own number to every file
                fieldno(i) = getfield(files(i), 'number'); %extract all numbers
            end
        end

    
    %% Clear
    clear; close all; clc; %clear all variables, close all figures, clear console
    disp('Kernel started.') %display start notification in console
    
    %% Settings
    %  Filename
    FILENAME = 'step_'; %naming scheme of the export files
    
    %  Cross-section of velocity magnitude
    CROSS_SECTION  = 'z'; %plane of velocity cross-section {x, y, z} = const.
    CROSS_FRACTION = 0.5; %location of velocity cross-section [0 1]
    
    %  Formatting
    CBAR_SCALE = 'auto'; %scaling of color bar: either [lower, upper] or 'auto'
    STREAMLINES = false; %streamlines on (true) / off (false)
    VORTICITY = false; % vorticity contours on (true) / off (false)
    PRESSURE = false; %draw pressure contours (true/false)
    LABELS = false; % draw axis labels (true/false)
    VISUALISATION_RESOLUTION = [100 200 800 600]; %resolution of the velocity magnitude window
    
    %  Residuals
    RESIDUALS = true; %residuals on (1) / off (0)
    PROFILE_CROSS_SECTION = true; %velocity profile cross-section
    X_CROSS = 0.5; %relative location of cross section ]0,1] in y-direction (x=const.)
    Y_CROSS = 0.5; %relative location of cross section ]0,1] in x-direction (y=const.)
    RESIDUALS_RESOLUTION = [900 200 800 600]; %resolution of the residuals window   
    
    %  Export
    VIDEO = false; %create video from figures (true/false), set this to false for Octave
    IMAGE_SEQUENCE = false; %create image sequence from figures (true/false)
    
    %  Other
    TMAX = 5; %maximum time without new file
    IMPORT_GEOMETRY = false; %import (true/false) walls (very slow!)
    
    
    %% Deactivate incompatible features (isOctave.m)
    if (isOctave() == true)
      disp('  Warning: You seem to be using Octave. Certain features will be de-activated.') %display compatibility warning in console
      VIDEO = false;
      IMAGE_SEQUENCE = false;
    end
    
    %% Import parameters from file (ParametersImport.m)
    try
        [NX, NY, NZ, NT, RE, RHO0, L, U] = ParametersImport('BIN/parameters.txt'); %import characteristic parameters
    catch
        disp('ERROR: No Parameters file found.') %display error message in console
        return %quit kernel
    end
    
    %% Import geometry from file
    try %try to find geometry
        BounceBack = GeometryImport(IMPORT_GEOMETRY); %import geometry (GeometryImport.m)
    catch
        disp('  Warning: No geometry file found. Kernel quit.') %display error message in console
        BounceBack = false(NX, NY, NZ);
    end
    
    %% Crop domain to relevant part
    if (strcmp(CROSS_SECTION, 'z') == 1)
        BounceBack_cross = squeeze(BounceBack(:, :, round(CROSS_FRACTION*NZ)))'; %correct orientation for plotting
    elseif (strcmp(CROSS_SECTION, 'y') == 1)
        BounceBack_cross = squeeze(BounceBack(:, round(CROSS_FRACTION*NY), :))';
    elseif (strcmp(CROSS_SECTION, 'x') == 1)
        BounceBack_cross = squeeze(BounceBack(round(CROSS_FRACTION*NX), :, :))';
    end
    
    % Velocity cross-section depending on orientation
    if (strcmp(CROSS_SECTION, 'z') == 1)
        X_CROSS = round(X_CROSS*NX); %calculate coordinates of cross-section in pixel
        Y_CROSS = round(Y_CROSS*NY);
    elseif (strcmp(CROSS_SECTION, 'y') == 1)
        X_CROSS = round(X_CROSS*NX);
        Y_CROSS = round(Y_CROSS*NZ);
    elseif (strcmp(CROSS_SECTION, 'x') == 1)
        X_CROSS = round(X_CROSS*NY);
        Y_CROSS = round(Y_CROSS*NZ);
    end
    
    BounceBack_x = repelem(BounceBack_cross(:, X_CROSS),1, 100); %get relevant bounce-back y-direction and extend to matrix
    BounceBack_y = repelem(BounceBack_cross(Y_CROSS, :),100, 1); %get relevant bounce-back x-direction and extend to matrix
        
    disp('- Geometry imported successfully.') %display notification in console
    disp('- Processing files...') %display notification in console
    
    %% Set-up plots and export
    fig = 1; %figure for plot
    res = 2; %figure for residuals
    
    figure(fig); %open plot figure
    if RESIDUALS==1
        figure(res); %open figure
    end
    
    % Simulation figure properties
	  figure(fig); %open figure
    set(fig, 'Position', VISUALISATION_RESOLUTION, 'MenuBar', 'none', 'ToolBar', 'none', ...
            'Name', 'LBM Simulation', 'NumberTitle', 'off') %define position and size of figure
    if (RESIDUALS == true)
        figure(res); %open figure
        set(res, 'Position', RESIDUALS_RESOLUTION, 'MenuBar', 'none', 'ToolBar', 'none', ...
                'Name', 'LBM Simulation', 'NumberTitle', 'off') %define position and size of figure
    end
    
    % Set-up plots
    try
        set(groot, 'DefaultTextInterpreter', 'LaTex'); %set LaTex as default interpreter for text
        set(groot, 'DefaultAxesTickLabelInterpreter', 'LaTex'); %... for axes
        set(groot, 'DefaultColorbarTickLabelInterpreter', 'LaTex'); %... for colorbar
        set(groot, 'DefaultLegendInterpreter', 'LaTex'); %... for legends
    catch
        
    end
    
    % Set-up Export
    if (VIDEO == true) %create video in loop
        mov = VideoWriter([pwd '\BIN\Simulation.avi']); %name and location for output-file
        mov.FrameRate = 30; %frame rate for the video
        mov.Quality = 100; %maximum video quality
        open(mov); %open output-file
        disp('- Recording video to file.') %notification in console
    end
    
    if (IMAGE_SEQUENCE == true) %create image sequence
        if (exist([pwd '/images'], 'dir') == 0) %if directory isn't there
            mkdir(pwd, 'images') %make new directory
        end
    end

    [xx, yy] = meshgrid(0.5:1:(NX+0.5-1), 0.5:1:(NY+0.5-1)); %create meshgrid for visualisation
    
    %% Start file import
    fieldno = ImportNumber(FILENAME); %import currently exported *-bin-file numbers
    currno = min(fieldno); %start with first file
    [no, ~] = size(fieldno); %number of elements
    tplot = (max(fieldno) - min(fieldno))/(no-1); %calculate plot step
    
    j = 1; %counter for image sequence
    tic; %start counter
    while (toc < TMAX) %as long as time between two created frames does not exceed tmax seconds
        
        fieldno = ImportNumber(FILENAME); %import new file list
        while currno < max(fieldno) %loop through
            relfield = fieldno(find(fieldno >= currno)); %elements closest and larger to value
            [~, index] = min(abs(relfield - currno)); %find closest index in relevant files
            [~, index] = min(abs(fieldno - relfield(index))); %find corresponding index in all files
            [FILENAME num2str(fieldno(index))]
            [rho, ux, uy, uz] = StepImport(NX, NY, NZ, [FILENAME num2str(fieldno(index))]); %import from binary file (external function VelocityImport.m)
            
            try
                ux = ux./U; %try to rescale values
                uy = uy./U;
                uz = uz./U;
            catch
                %if U not imported successfully: do no rescale
            end
            
            if (ishandle(fig) == 0) %check if figure was closed (by user)
                if (VIDEO == true)
                    disp('- Video export failed.') %notification in console
                end
                disp('ERROR: Operation terminated by user. Kernel quit.') %display error message in console
                return %quit kernel
            end
            
            % get relevant values
            if (strcmp(CROSS_SECTION, 'z') == 1)
                data_x = squeeze(ux(:, :, round(CROSS_FRACTION*NZ)));
                data_y = squeeze(uy(:, :, round(CROSS_FRACTION*NZ)));
                data_z = squeeze(uz(:, :, round(CROSS_FRACTION*NZ)));
                data_rho = squeeze(rho(:, :, round(CROSS_FRACTION*NZ)));
                width = NX;
                height = NY;
            elseif (strcmp(CROSS_SECTION, 'y') == 1)
                data_x = squeeze(ux(:, round(CROSS_FRACTION*NY), :));
                data_y = squeeze(uz(:, round(CROSS_FRACTION*NY), :));
                data_z = squeeze(uy(:, round(CROSS_FRACTION*NY), :));
                data_rho = squeeze(rho(:, round(CROSS_FRACTION*NY), :));
                width = NX;
                height = NZ;
            elseif (strcmp(CROSS_SECTION, 'x') == 1)
                data_x = squeeze(uy(round(CROSS_FRACTION*NX), :, :));
                data_y = squeeze(uz(round(CROSS_FRACTION*NX), :, :));
                data_z = squeeze(ux(round(CROSS_FRACTION*NX), :, :));
                data_rho = squeeze(rho(round(CROSS_FRACTION*NX), :, :));
                width = NY;
                height = NZ;
            end
            
            if (currno == min(fieldno)) %first loop
                data_x_old = data_x; %set default values
                data_y_old = data_y;
            end
            
            Visualisation(fig, res, RHO0, ...
                          data_x, data_y, data_z, data_rho, ...
                          BounceBack_cross, xx, yy, width, height, ...
                          STREAMLINES, VORTICITY, PRESSURE,...
                          CBAR_SCALE, LABELS, ...
                          PROFILE_CROSS_SECTION, X_CROSS, Y_CROSS); %visualise (external function Visualisation.m)
                      
            Residuals(res, data_x, data_y, data_x_old, data_y_old, tplot, width, height, ...
                      PROFILE_CROSS_SECTION, X_CROSS, Y_CROSS, BounceBack_x, BounceBack_y);
            
            if (VIDEO == true) %make video
                try %if figure was closed
                    Fa = getframe(fig); %grab frames
                    writeVideo(mov, Fa); %write frames to video
                catch
                    disp('- Video export failed.') %notification in console
                    disp('ERROR: Operation terminated by user. Kernel quit.') %display error message in console
                    return %quit kernel
                end
            end
            
            if (IMAGE_SEQUENCE == true) %make image sequence
                imname = sprintf('%06d', j); %create filename
                fullname = fullfile(pwd, 'images', imname); %create complete filename
                saveas(fig, fullname, 'jpg');
                j = j+1; %keep counting
            end
            
            data_x_old = data_x; %set old velocity matrices
            data_y_old = data_y;
            
            fclose all; %close all files
            
            currno=fieldno(index)+1; %next index to look for = last index + 1
            tic; %restart timer
            %pause(0.01) %pause for defined time
        end
    end
    
    if (VIDEO == true) %finalise video
        close(mov); %close mov file
        disp('- Video export successful.') %notification in console
    end
    
    disp('- Time limit passed. No more files found.')
    disp('Kernel quit.')
    
    %% Export to vector image
    %set(1,'Position',[100 100 600 600]) %rescale image
    %set(0,'DefaultFigureWindowStyle','normal') %undo toggle (in case window is toggled)
    %print -painters -dsvg filename.svg %export as vector image using the exporter painters

end
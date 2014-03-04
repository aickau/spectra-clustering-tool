Welcome to ASPECT - A SPEctra Clustering Tool for the exploration of large spectroscopic surveys

Analysing the output from surveys is an important challenge in modern science. This, however, is not an easy task for a voluminous dataset.

We developed the software tool ASPECT which is based on a novel approach for analysing large spectra databases. The heart of the software is a Kohonen self-organizing map (SOM) that is able to cluster up to several hundred thousand spectra. The resulting two-dimensional SOM of topologically ordered spectra allows the user to browse and navigate through a huge data pool and helps him to gain a deeper insight into underlying relationships between spectral features and other physical properties.

ASPECT supports the visual analysis of the resulting SOM of spectra by the option to blend in individual object properties (e.g., redshift, signal-to-noise ratio, spectral type, morphological type). In addition, the tool provides algorithms for the selection of spectral types, e.g., local difference maps which reflect the deviations of all spectra from a given input spectrum. 


For a detailed description you might want to look into this paper:
ASPECT: A spectra clustering tool for exploration  of large spectral surveys
A.  in der Au, H.  Meusinger, P. F.  Schalldach, M.  Newholm
A&A 547 A115 (2012)
DOI: 10.1051/0004-6361/201219958

A special application is descibed here:
Unusual quasars from the Sloan Digital Sky Survey selected by means of Kohonen self-organising maps⋆
H.  Meusinger, P.  Schalldach, R.-D.  Scholz, A.  in der Au, M.  Newholm, A.  de Hoon, B.  Kaminsky
A&A 541 A77 (2012)
DOI: 10.1051/0004-6361/201118143



How to cluster spectral data:
=============================

(1) The spectra we want to cluster have to be preprocessed and must be packed into a single binary file. For this purpose use the dump tool, for example:

dumpx64Release.exe -d c:/fitsfiles/* -o allSpectra.bin 

This would generate the file allSpectra.bin with all SDSS spectra that are located in c:/fitsfiles (and subdirectories).
The log file dump_log.txt is written and contains additional information if something goes wrong, e.g. spectrum files could not be loaded, etc.

(2) Cluster the spectra by using the analyze tool:

analyzerx64Release.exe -i allSpectra.bin 

additional configuration options can be tweaked in settings.xml. The log file sofm_log.txt contains additional informaion about the clustering process. The clustering process can be aborted at any time. 
When starting with analyzerx64Release.exe -c -i allSpectra.bin (notice the -c option) again the computation continues at the last learning step. You may want to backup the log file and the the entire network configuration written to sofmnet.bin for the last learning step. 


For each learning step we write an intermediate HTML map to export/current.html. The final map is written to export/full.html.



Configration options explained (in settings.xml):
=================================================

<STEP current="200" total="200"/> 
Contains the current learning step and the total number of learning steps. The current learning step is more like a state and modified during clustering. The toal number of learning steps can be adjusted. Values betwenn 100 and 1000 are useful. 

<GRIDSIZE value="6"/>
The gridsize is automatically calculated during startup regarding the number of input spectra:
GRIDSIZE = ceil(sqrt(num_input_spectra+1))*1.1
Adjustements by the user are ignored.

<RANDOMSEED value="15"/>
For randomization purposes.

<LEARNRATE begin="0.25" end="0.01"/>
Adaption rate of the network for the first and last learning steps (0.0 .. 0.5). The learning rate decreases inverse exponentially.

<RADIUS begin="1" end="0.125"/>
Adaption radius 1.0=entire map. The radius decreases inverse exponentially.

<SPECTRUM file="sofmnet.bin"/>
Filename where the entire network configuration is written to after each learning step.

Export settings:

<SUBPAGES value="1"/> [0,1]
If set to 1 the exported HTML output is subdivided into 15x15 subpages. 

<WAITFORUSER value="0"/> [0,1]
Wait for user input after each learning step.

<FULLEXPORT value="0"/> [0,1]
Calculate additional comparison information for each spectrum. Not suited for very large maps with more than 100.000 spectra.

<IMGBORDERSIZE value="6"/> 
Size in pixels for the image border around the spectra.

<ICONSIZE value="64"/>
Icon size in pixels for all spectra icons that are rendered during startup. Make sure to the delete the icons in export/ if icons should be rendered again. 

<SEARCHMODE value="local"/><!-- localfast / local / global -->
How the best matching unit (BMU) is found in a map. For smaller maps use global where every spectrum in the map is considered for search. For larger maps use local where BMUs are only searched in their map vicinity.

<NORMALIZATION value="none"/><!-- amplitude / flux / none -->
Normalization mode for spectra (flux should be default). When using flux each spectrum is normalized by its total energy. When using amplitude each spectrum is normalized by the maximum amplitude (not useful for SDSS spectra). 

<WAVELENGTHRANGE value="boss"/><!-- sdss (3800..9200) / boss (3650..10400) -->
Wavelength range in Angström. Use boss for spectra of data release 9 and above.




This directory layout is organized in the following form:
=========================================================

/data                  -  includes some example spectrum files we can import
/export                -  where the exported (HTML) map is written too
/src/3rdparty          -  3rd party libraries - for a detailed description see /src/3rdparty/layout.txt
/src/analyze           -  this is where all the magic happens: calculate SOM and export the clustered map from precompiled list of binary spectra
/src/compare           -  compare one spectrum with clustered map to find similar objects and other sophisticatd foo
/src/dump              -  precompiled spectra (i.e. spectra FITS files from varios SDSS data releases) into a single binary file
/src/sdsslib           -  collection of shared library functions that are used by ASPECT
/src/specObjOperations -  a collection of various special purpose test code 
/src/specObjMapper     -  to plot many spectra into one graph to visually compare them
/src/search            -  perform search queries to find similar spectra for a list of given spectra

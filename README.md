# POIGPS
Point of interest GPS. This program finds locations with a given keyword within a radius.
The bash command is as follows:

       ./search-map <lat> <lon> <radius> <filter> <database>
        <lat>           Latitude of query point
        <lon>           Longitude of query point
        <radius>        Radius of query
        <filter>        Single word to filter results (no whitespaces)
        <database>      File name containing all the locations for a given region
Example: find all places that contain 'restaurant' under a radius of 10 miles from 42.34,-71.10 in boston.txt
        ./search-map 42.34 -71.10 10 restaurant boston.txt
        

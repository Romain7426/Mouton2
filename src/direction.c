#include "global.h"
#include "direction.h"


real direction_x_get(direction_t d) {
  switch (d)
    {
    case dBAS: return 0.0;
    case dDROITE: return 1.0;
    case dHAUT: return 0.0;                 
    case dGAUCHE: return -1.0;   
    case dRIEN: return 0.0; 
    default: return 0.0;   
    }   
  
}



real direction_y_get(direction_t d) {
  switch(d)
    {
    case dBAS: return -1.0;
    case dDROITE: return 0.0;
    case dHAUT: return 1.0;                 
    case dGAUCHE: return 0.0;   
    case dRIEN: return 0.0;
    default: return 0.0;     
    }          
  
}

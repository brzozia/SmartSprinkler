import React from 'react';
import { Typography, AppBar, Toolbar} from '@material-ui/core';

class Navbar extends React.Component{
    render(){
        return(
              <AppBar position="static" style={{"backgroundColor":"#607d8b"}}>
              <Toolbar>
              <Typography variant="h6" >
                  Smart sprinkler
              </Typography>
              </Toolbar>
          </AppBar>
        );
            }
}

export default Navbar;
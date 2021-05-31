import React from 'react';
import { Typography, Box, AppBar, Toolbar} from '@material-ui/core';

class Navbar extends React.Component{
    render(){
        return(
              <AppBar position="static" style={{"backgroundColor":"teal"}}>
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
import React from 'react';
import ReactDOM from 'react-dom';
import Grid from '@material-ui/core/Grid';
import Card from '@material-ui/core/Card';
import './index.css';
import Status from './components/Status.js'
import Navbar from './components/Navbar.js'
import Sensors from './components/Sensors.js'
import TableComponent from './components/TableComponent'
import { CardContent, Box, CardHeader, Divider } from '@material-ui/core';
import Strategy from './components/Strategy';
import AddStrategy from './components/AddStrategy';


  class Dashboard extends React.Component {
    render() {
      return (
        <div className="dashboard">
            <Box mb={3}>
                 <Navbar />
             </Box>
            <Box pr={3} pl={3}>
            <Grid container>
                 <Grid item sm={3} xs={12} container >
                    <Status/>   
                </Grid>

                <Grid item sm={3} xs={12} container >
                    <Grid item xs={12}>
                        <Card>
                            <CardHeader title="Working strategy"/>
                            <Divider variant="middle"/>
                            <CardContent>
                                <Strategy strategy="strategy1"/>
                            </CardContent>
                        </Card>
                    </Grid>
                </Grid>

                <Grid item sm={6} xs={12} container>
                    <Sensors />
                 </Grid>
            </Grid> 

            <Grid container>
                <Grid item md={5} xs={12}>
                    <Box m={2}>
                        <TableComponent/>
                    </Box>
                </Grid>
                <Grid item md={7} xs={12}>
                    <Box m={2}>
                        <AddStrategy/>
                    </Box>
                </Grid>
            </Grid>
            </Box>
        </div>
 
      );
    }
  }
  
  // ========================================
  
  ReactDOM.render(
    <Dashboard />,
    document.getElementById('root')
  );
  
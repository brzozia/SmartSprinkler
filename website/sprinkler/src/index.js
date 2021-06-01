// require('file-loader?name=[name].[ext]!./index.html');
import React from 'react';
import ReactDOM from 'react-dom';
import Grid from '@material-ui/core/Grid';
import Card from '@material-ui/core/Card';
import './index.css';
import Status from './components/Status.js'
import Activities from './components/Activities.js'
import Navbar from './components/Navbar.js'
import TableComponent from './components/TableComponent'
import { CardContent, Typography, Box, CardHeader, Divider } from '@material-ui/core';


class DataCard extends React.Component {
    render() {
      return (
        <Card>
            <CardContent>
                <Typography variant="body1" color="textSecondary">last update 5 min ago</Typography>
                <Grid container justify="space-between">
                    <Grid item>
                     <Typography variant="h6" align="left">{this.props.title} </Typography>
                    </Grid>
                    <Grid item>
                        <Typography variant="h5" align="right" >{this.props.value}</Typography>
                    </Grid>
                </Grid>
            </CardContent>
        </Card>
      );
    }
  }

  

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
                    <Grid item xs={12}>
                        <Box m={2}>
                            <Status working="true" start="31.05.2021 05:41" end="31.05.2021 6:00" lastStart="30.05.2021 05:41" lastEnd="3o.05.2021 6:00" strategyName="strategy1"/>
                        </Box>
                    </Grid>
                    <Grid item xs={12}>
                        <Box m={2}>
                            <Activities working="true"/>
                        </Box>
                    </Grid>
                </Grid>

                <Grid item sm={3} xs={12} container >
                    <Grid item xs={12}>
                        <Card>
                            <CardHeader title="Working strategy"/>
                            <Divider variant="middle"/>
                            <CardContent>
                            </CardContent>
                        </Card>
                    </Grid>
                </Grid>

                <Grid item sm={6} xs={12} container>
                    <Grid item md={12} container direction="row">
                        <Grid item xs>
                            <DataCard title="Temperature" value="23.3 C"/>
                        </Grid>
                        <Grid item xs>
                            <DataCard title="Soil humidity" value="4"/>
                        </Grid>
                    </Grid>

                    <Grid item md={12} container direction="row">
                        <Grid item xs>
                            <DataCard title="Air humidity" value="60"/>
                        </Grid>
                        <Grid item xs>
                            <DataCard title="Rain probability" value="80%"/>
                        </Grid>
                    </Grid>
                    <Grid item md={12} container direction="row">
                        <Grid item xs>
                            <DataCard title="Wind speed mean" value="40 km/h"/>
                        </Grid>
                    </Grid>
                </Grid>
            </Grid>

            <Grid container>
                <Grid item xs={8}>
                    <Box m={2}>
                        <TableComponent/>
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
  
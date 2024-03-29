// require('file-loader?name=[name].[ext]!./index.html');
import React from 'react';
// window.React = React;

import ReactDOM from 'react-dom';
// window.ReactDOM = ReactDOM;
import {Grid} from '@material-ui/core';
// window.Grid = Grid;
// window.Card = Card;
import './index.css';
import Status from './components/Status.js'
import Navbar from './components/Navbar.js'
import Sensors from './components/Sensors.js'
import TableComponent from './components/TableComponent'
import MaterialUI from '@material-ui/core';

import {Box } from '@material-ui/core';
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
                    <Status/>  

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
  
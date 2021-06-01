import React from 'react';
import { FormControl, InputLabel, Select, Icon, IconButton, TextField, Button, Grid, Box} from '@material-ui/core';
import { T, D, C, connectorTypes } from '../dicts';




class AddStrategy extends React.Component {
    constructor(props){
        super(props);

        this.state = {
            strategy: [],
            T: "",
            D: "",
            C: "",
            V: 0,
        }
        this.handleTChange = this.handleTChange.bind(this);
    }

    handleTChange = (e) =>{
        this.setState(() => {return ({T: e.target.value})}, console.log(this.state.T));
        this.forceUpdate();
    }
    handleDChange = (e) =>{
        this.setState({D: e.target.value});
    }
    handleCChange = (e) =>{
        this.setState({C: e.target.value});
    }
    handleVChange = (e) =>{
        this.setState({V: e.target.value});
    }
    
    render(){
        return(
            <>
            <Grid container spacing={1}>
                <Grid item xs={3}>
                    <FormControl variant="outlined">
                        <InputLabel>Type</InputLabel>
                        <Select
                        native
                        value={this.state.T}
                        onChange={this.handleTChange}
                        label="Type"
                        >
                        {Object.keys(T).map((item, i) => {
                            return (
                            <option key={i} value={item}> {T[item.toString()]} </option>
                            );
                        })}
                        </Select>
                    </FormControl>
                </Grid>
                    
                <Box display={this.state.T===1 ? "block" : "none"}>
                <Grid  item xs={3}>
                    <FormControl ariant="outlined">
                        <InputLabel>Data</InputLabel>
                        <Select
                        native
                        value={this.state.D}
                        onChange={this.handleDChange}
                        label="Data"
                        >
                        {Object.keys(D).map((item, i) => {
                            return (
                            <option key={i} value={item}> {D[item.toString()]} </option>
                            );
                        })}
                        </Select>
                    </FormControl>
                </Grid>
                </Box>
                
                <Box display={this.state.T===1 ? "block" : "none"}>
                <Grid item xs={2}>
                    <FormControl variant="outlined">
                        <InputLabel>Condition</InputLabel>
                        <Select
                        native
                        value={this.state.C}
                        onChange={this.handleCChange}
                        label="Condition"
                        >
                        {Object.keys(C).map((item, i) => {
                            return (
                            <option key={i} value={item}> {C[item.toString()]} </option>
                            );
                        })}
                        </Select>
                    </FormControl>
                </Grid>
                </Box>

                <Box display={this.state.T===2 ? "none" : "block"}>
                <Grid item xs={2}>
                    <FormControl variant="outlined">
                        <form noValidate autoComplete="off">
                            <TextField id="outlined-basic" 
                            value={this.state.V} 
                            onChange={this.handleVChange} 
                            label="Value" 
                            variant="outlined" />
                        </form>
                    </FormControl>
                </Grid>
                </Box>
                
                <Box display={this.state.T===2 ? "block" : "none"}>
                <Grid item xs={2}>
                    <FormControl variant="outlined">
                        <InputLabel>Connection</InputLabel>
                        <Select
                        native
                        value={this.state.Connection}
                        onChange={this.handleConnectionChange}
                        label="Connection"
                        >
                        {Object.keys(connectorTypes).map((item, i) => {
                            return (
                            <option key={i} value={item}> {connectorTypes[item.toString()]} </option>
                            );
                        })}
                        </Select>
                    </FormControl>
                </Grid>
                </Box>

                <Grid item xs={2}>
                <IconButton aria_label="add watering" onClick={() => this.addRow()}>
                        <Icon fontSize="large">add_circle</Icon>
                    </IconButton>
                </Grid>
            </Grid>
            <Grid>
                <Box pt={2}>
                <Button variant="contained">Submit</Button>
                </Box>
            </Grid>
        </>
        );
    }
}

export default AddStrategy;
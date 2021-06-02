import React from 'react';
import { FormControl, InputLabel, Select, Icon, Card, IconButton, TextField, Button, Grid, Box, CardHeader, CardContent} from '@material-ui/core';
import { T, D, C, connectorTypes, urls } from '../dicts';


class StrategyRow extends React.Component{
    constructor(props){
        super(props);

        this.state = {
            T: "1",
            D: "",
            C: "",
            V: 0,
        }
    }

    handleTChange = (e) =>{
        this.setState(() => {return ({T: e.target.value})});
        this.props.handleTChange(e.target.value);
    }
    handleDChange = (e) =>{
        this.setState({D: e.target.value});
        this.props.handleDChange(e.target.value);
    }
    handleCChange = (e) =>{
        this.setState({C: e.target.value});
        this.props.handleCChange(e.target.value);
    }
    handleVChange = (e) =>{
        this.setState({V: e.target.value});
        this.props.handleVChange(e.target.value);
    }

    render(){
        return(
            <>
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
                    
                <Grid item  xs={3} className={this.state.T==="1" ? "" : "none"}>
                    <FormControl variant="outlined">
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
                
                <Grid item  xs={2} className={this.state.T==="1" ? "" : "none"}>
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

                <Grid item xs={2} className={this.state.T==="2" ? "none" : ""}>
                    <FormControl variant="outlined"> 
                        <form noValidate autoComplete="off">
                            <TextField  
                            value={this.state.V} 
                            onChange={this.handleVChange} 
                            label="Value" 
                            variant="outlined" />
                        </form>
                  </FormControl>
                </Grid>
                
                <Grid item  xs={2} className={this.state.T==="2" ? "" : "none"}>
                    <FormControl variant="outlined">
                        <InputLabel>Connection</InputLabel>
                        <Select
                        native
                        value={this.state.C}
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
            </>);
    }
}

class AddStrategy extends React.Component {
    constructor(props){
        super(props);

        this.state = {
            strategy: [],
            name: " ",
            interval: 5,
            T: "1",
            D: "",
            C: "",
            V: 0,
        }
        this.handleTChange = this.handleTChange.bind(this);
        this.handleDChange = this.handleDChange.bind(this);
        this.handleCChange = this.handleCChange.bind(this);
        this.handleVChange = this.handleVChange.bind(this);
    }

    handleTChange = (e) =>{
        this.setState(() => {return ({T: e})}, console.log(this.state.T));
    }
    handleDChange = (e) =>{
        this.setState({D: e});
    }
    handleCChange = (e) =>{
        this.setState({C: e});
    }
    handleVChange = (e) =>{
        this.setState({V: e});
    }
    handleName = (e) =>{
        this.setState({name: e.target.value});
    }
    handleInterval = (e) =>{
        this.setState({interval: e.target.value});
    }

    parseInput(){
        let obj = {};
        obj.T = this.state.T;
        switch (this.state.T){
            case "1":
                obj.D = this.state.D;
                obj.C = this.state.C;
                obj.V = this.state.V;
                break;
            case "2":
                obj.C = this.state.C;
                break;
            case "3":
                obj.V = this.state.V;
                break;
        }
        return obj;
    }

    addRow(){
        let obj = this.parseInput();
        this.setState((state) => {
            return({
                strategy: [...state.strategy, obj],
                T: "1",
                D: "",
                C: "",
                V: 0,
                name: " ",
                interval: 5,
            })
        })
        return (<StrategyRow handleCChange={this.handleCChange} 
            handleDChange={this.handleDChange} 
            handleTChange={this.handleTChange} 
            handleVChange={this.handleVChange}
        />)
    }

    submit(){
        this.addRow();
    
        let formData = new FormData();
        formData.append('name', this.state.name);
        formData.append('body', this.state.strategy);

        fetch(urls.getStrategies, {
            method:"POST", 
            body: formData
        }).catch(err => console.log(err));
    }

    
    render(){
        return(
            <>
            <Card>
                <CardHeader title="Add strategy"/>
                <CardContent>
                    <Grid container spacing={1} style={{"marginBottom":"1rem"}}>
                        <Grid item xs={3}>
                                <FormControl variant="outlined"> 
                                    <form noValidate autoComplete="off">
                                        <TextField  
                                        value={this.state.name} 
                                        onChange={this.handleName} 
                                        label="Name" 
                                        variant="outlined" />
                                    </form>
                            </FormControl>
                        </Grid>
                        <Grid item xs={2} >
                            <FormControl variant="outlined"> 
                                <form noValidate autoComplete="off">
                                    <TextField  
                                    value={this.state.interval} 
                                    onChange={this.handleInterval} 
                                    label="Interval [min]" 
                                    variant="outlined" />
                                </form>
                        </FormControl>
                        </Grid>
                    </Grid>
                    
                    <Grid container spacing={1} >

                        <StrategyRow handleCChange={this.handleCChange} 
                            handleDChange={this.handleDChange} 
                            handleTChange={this.handleTChange} 
                            handleVChange={this.handleVChange}
                        />

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
                </CardContent>
            </Card>
            
        </>
        );
    }
}

export default AddStrategy;
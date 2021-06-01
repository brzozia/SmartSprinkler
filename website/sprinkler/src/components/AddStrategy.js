import React from 'react';
import { FormControl, InputLabel, Select} from '@material-ui/core';
import { T, urls } from '../dicts';




class AddStrategy extends React.Component {
    constructor(props){
        super(props);

        this.state = {
            strategy: [],
            T: "",
            D: "",
            C: "",
            V: "",
        }
    }

    handleTChange = (e) =>{
        this.setState({T: e.target.value});
    }
    
    render(){
        return(
            <>
        <FormControl variant="outlined">
            <InputLabel>Type</InputLabel>
            <Select
            native
            value={this.state.T}
            onChange={this.handleTChange}
            label="Type"
            >
            <option aria-label="None" value="" />
            {Object.keys(T).map((item, i) => {
                return (
                <option key={i} value={item}> {T[item.toString()]} </option>
                );
            })}
            </Select>
        </FormControl>
        <FormControl variant="outlined">
            <InputLabel>Type</InputLabel>
            <Select
            native
            value={this.state.T}
            onChange={this.handleTChange}
            label="Type"
            >
            <option aria-label="None" value="" />
            {Object.keys(T).map((item, i) => {
                return (
                <option key={i} value={item}> {T[item.toString()]} </option>
                );
            })}
            </Select>
        </FormControl>
        </>
        );
    }
}

export default AddStrategy;
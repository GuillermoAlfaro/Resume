import React, { FunctionComponent } from 'react'
import styled from 'styled-components/native'

import {colors} from "../colors"
const { primary, background, secondary, black } = colors;

const StyledText = styled.Text `
    font-size: 19px;
    color: black ;
    text-align: left;
`

import { TextProps } from './types';

const RegularText: FunctionComponent<TextProps> = (props) => {
    return <StyledText style={props.style}>{props.children}</StyledText>
};

export default RegularText


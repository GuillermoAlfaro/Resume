import React, { FunctionComponent } from 'react'
import styled from 'styled-components/native'

import {colors} from "../colors"
const { black } = colors;

const StyledText = styled.Text `
    font-size: 30px;
    color: black;
    text-align: left;
`

import { TextProps } from './types';

const BigText: FunctionComponent<TextProps> = (props) => {
    return <StyledText style={props.style}>{props.children}</StyledText>
};

export default BigText


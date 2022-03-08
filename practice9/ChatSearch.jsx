import React, { useState } from "react";
import styled from "styled-components";
import TextField from "@material-ui/core/TextField";

// 채팅 키워드 검색창 & 검색 버튼에 대한 컴포넌트 작성
function ChatSearch({chats, onSearchKeySubmit}) {
    const [keyword, setKeyword] = useState("")

    const onKeywordChange = (e) => {
        setKeyword(e.target.value);
        // console.log("keyword :", keyword);
    }

    const onKeywordSubmit = (e) => {
        e.preventDefault()

        for(let i = 0; i < chats.length; i++) {
            chats[i].searched = false;
        }

        // console.log("keyword! :", keyword);
        onSearchKeySubmit(keyword);
        setKeyword("");
        // console.log("keyword!! :", keyword);
    }

    return (
        <SearchForm onSubmit={onKeywordSubmit}>
            <SearchMessage>
                <TextField
                    name="keyword"
                    onChange={(e) => onKeywordChange(e)}
                    value={keyword}
                    id="outlined-multiline-static"
                    variant="outlined"
                    label="Keyword"
                    fullWidth="true"
                />
            </SearchMessage>
            <SearchButton>Search</SearchButton>
        </SearchForm>
    );
}

const SearchForm = styled.form`
  width: 460px;
  height: 50px;
  padding: 20px;
  border-radius: 5px;
  background-color: #f3eaaf;
`;

const SearchMessage = styled.div`
   width:380px;
   vertical-align: middle;
   float: left;
`;

const SearchButton = styled.button`
  padding: 10px;
  background-color: white;
  border-color: #c8a951;
  border-radius: 10px;
  margin-top:7px;
  float: right;
`;

export default ChatSearch;
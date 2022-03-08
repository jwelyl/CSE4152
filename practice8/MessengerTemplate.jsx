import React, { useState } from "react";
import styled from "styled-components";
import TextField from "@material-ui/core/TextField";

// Sending Message 부분
function MessenagerTemplate({getChatLog}) {
  const [ MsgState, setMsgState ] = useState({ message: "", username: ""});

  /*const onMsgChange = (e) => {
		setMsgState({ ...MsgState, [e.target.name]: e.target.value });
	}*/

  const onUsernameChange = (e) => {
    MsgState.username = e.target.value;
  }

  const onMessageChange = (e) => {
    MsgState.message = e.target.value;
  }

  // 메시지 입력 창에서 빈 경우에는 ‘메시지를 입력하세요‘라는 내용의 경고 창 출력 후 리턴(alert 함수 사용)
  // 사용자의 이름 입력 창이 빈 경우에는 사용자 이름을 ‘이름없음’으로 저장
  // MsgState의 속성(키)들의 값을 빈 문자열로 초기화
	const onMsgSubmit = (e) => {
    e.preventDefault();
    // 8주차 실습 구현

    //  Debugging
    console.log("message in onMsgSubmit: ", MsgState.message);
    console.log("username in onMsgSubmit: ", MsgState.username);
    setMsgState({ ...MsgState, [e.target.name]: e.target.value });

    if(!MsgState.message) {
      alert("메시지를 입력하세요.");
      return;
    }
    if(!MsgState.username) {
      MsgState.username = "이름없음";
    }
    console.log("message in onMsgSubmit2: ", MsgState.message);
    console.log("username in onMsgSubmit2: ", MsgState.username);
    getChatLog(MsgState.username, MsgState.message);
    
    MsgState.username="";
    MsgState.message="";
    //setMsgState({message: "", username: ""});


    console.log("message in onMsgSubmit3: ", MsgState.message);
    console.log("username in onMsgSubmit3: ", MsgState.username);
	}

  return (
      <Form onSubmit={onMsgSubmit}>
				{/* 8주차 실습 구현 */}
        <Title>Sending Message</Title>
        <UserName>
          <TextField 
            label="User Name"
            onChange={onUsernameChange}/>
        </UserName>
        <MessageContents>
          <TextField label="Message"
            onChange={onMessageChange}/>
        </MessageContents>
        <Button
          type="submit"
          onClick={onMsgSubmit}
        >
          Send
        </Button>
			</Form>
  );
}

// Sending Message 글씨 박스
const Title = styled.h1`
  margin: 10px;
  padding: 10px;
  // 해당 영역 모서리를 둥글게
  border-radius:6px 6px 6px 6px;
  // 해당 영역의 배경색 변경
  background-color:#bdb76b;
  // 해당 영역 안 텍스트 폰트색 변경
  color: white;
  // 8주차 실습 구현
`;

// 채팅 메시지 입력 form
const Form = styled.form`
  width: 500px;
  margin: 10px;
  padding: 20px;
  // 해당 영역 모서리를 둥글게
  border-radius: 20px 20px 20px 20px;
  // 해당 영역 모서리에 그림자
  box-shadow:4px 4px 10px gray;
  // 해당 영역의 배경색 변경
  background: #ffffe0;
  // 8주차 실습 구현
`;

// username 표시
const UserName = styled.div`
  margin: 40px;
`;

// 채팅 메시지 부분
const MessageContents = styled.div`
  margin: 30px;
`;

// send 버튼
const Button = styled.button`
  margin-top: 60px;
  padding: 10px;
  // 해당 영역 모서리를 둥글게
  border-radius:6px 6px 6px 6px;
  // 해당 영역 모서리색 변경
  border-color: #ffd700;
  // 해당 영역의 배경색 변경
  background: #ffffff;
  // 8주차 실습 구현
`;

export default MessenagerTemplate;
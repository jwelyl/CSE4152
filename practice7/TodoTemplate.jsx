import React, { useState, useRef } from "react";
import styled from "styled-components";


import WhatTodo from "./WhatTodo";
import DoneList from "./DoneList";
import DeletedList from "./DeletedList";

function TodoTemplate() {
  const [todos, setTodos] = useState([]);
  const [doneTodos, setDoneTodos] = useState([]);
  const [deletedTodos, setDeletedTodos] = useState([]);

  const nextId = useRef(0);

  // 하위 컴포넌트(WhatTodo -> TodoInsert 컴포넌트)까지 전달됨
  // TodoInsert 컴포넌트에서 할일 항목 제출할 때 호출됨
  const handleSubmit = (text) => {
    // todo 객체 생성
    const todo = {
      id: nextId.current,
      text,
      checked: false,
    };
    // todos 배열에 삽입
    setTodos(todos.concat(todo));
    nextId.current += 1;
  };

  // 주어진 What To Do 항목(체크안된 항목)의 id로 What To Do에서 해당 항목을 찾고 Deleted List에 추가한 후에, What To Do에서 제외
  const onRemoveTodo = (id) => {
    // id에 해당하는 항목을 todos 배열에서 찾음
    const deletedTodo = todos.find((todo) => todo.id === id);
    // 삭제된 todo -> deletedTodos에 저장
    setDeletedTodos([...deletedTodos, deletedTodo]);
    // filter 함수 사용해서 id에 해당하는 항목만 todos 배열에서 제외
    setTodos(todos.filter((todo) => todo.id !== id));
  };

  // 주어진 Done List 항목(체크된 항목)의 id로 Done List에서 해당 항목을 찾고 Deleted List에 추가한 후에, 기존 Done List에서 제외
  const onRemoveDoneTodo = (id) => {
    // 7주차 실습 구현
    //  id에 해당하는 항목을 doneTodos 배열에서 찾음
    const deletedDoneTodo = doneTodos.find((doneTodo) => doneTodo.id === id);
    //  삭제된 doneTodo -> deletedTodos에 저장
    setDeletedTodos([...deletedTodos, deletedDoneTodo]);
    //  filter 함수 사용해서 id에 해당하는 항목만 doneTodos 배열에서 제외
    setDoneTodos(doneTodos.filter((doneTodo) => doneTodo.id !== id));
  };

  // 주어진 What To Do 항목(체크안된 항목)을 찾아 항목 객체의 속성(키)을 사용하여 체크한 후, Done List에 추가하고 What To Do에서 제외
  const onToggleToDo = (id) => {
    // 7주차 실습 구현
    //  id에 해당하는 항목을 todos 배열에서 찾음
    let doneTodo = todos.find((todo) => todo.id === id);
    //  해당 항목의 checked를 true로 변경
    doneTodo.checked = true;
    //  checked된 doneTodo를 doneTodos에 저장
    setDoneTodos([...doneTodos, doneTodo]);
    //  filter 함수를 사용해서 id에 해당하는 항목만 todos 배열에서 제외
    setTodos(todos.filter((todo) => todo.id !== id));
  };

  // 주어진 Done List 안 항목을 찾아 항목 객체의 속성을 사용하여 체크 해제한 후, What To Do에 추가하고 Done List에서 제외
  const onToggleDoneToDo = (id) => {
    // 7주차 실습 구현
    //  id에 해당하는 항목을 doneTodos 배열에서 찾음
    let todo = doneTodos.find((doneTodo) => doneTodo.id === id);
    //  해당 항목의 checked를 false로 변경
    todo.checked = false;
    //  checked 해제된 todo를 todos에 저장
    setTodos([...todos, todo]);
    //  filter 함수를 사용해서 id에 해당하는 항목만 doneTodos 배열에서 제외
    setDoneTodos(doneTodos.filter((doneTodo) => doneTodo.id !== id));
  };

  return (
    <div>
      <Container>
        <Title>
          <h1>TODO LIST</h1>
        </Title>
        <Contents>
          <WhatTodo handleSubmit={handleSubmit} todos={todos} onRemove={onRemoveTodo} onToggle={onToggleToDo}/>
          {/* 구분선 위한 div*/}
          <div style={{'borderLeft': '2px solid', height: '400px', 'marginLeft': '10px'}}/>
          <DoneList doneTodos={doneTodos} onRemove={onRemoveDoneTodo} onToggle={onToggleDoneToDo} />
        </Contents>
      </Container>
      <footer>
        <DeletedList deletedTodos={deletedTodos} />
      </footer>
    </div>
  );
}

// user custom components
const Title = styled.div`
  color:#415e3e;
  margin: 20px;
  width: 250px;
  align-self: center;
  background-color: #f5c3c5;
  border-radius: 20px;
`;

const Container = styled.div`
  display: flex;
  flex-direction: column;  
  width: 1200px;
  height: 550px;
  border: 3px solid #000;
  border-radius: 20px;
  background-color: #fff;
`;

const Contents = styled.div`
  display: flex;
  flex-direction: row;
  justify-content: space-evenly;
`;

export default TodoTemplate;

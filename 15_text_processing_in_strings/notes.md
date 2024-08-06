# Regular Expression Library

## Finite Automaton
- state machine that can determine whether a given word belongs to a given
  language.
- a DFA can, at each state for a given valid input symbol, only end up in
  precisely one state.
- an NFA, then, is not limited in this way. given a particular state, and a
  valid input symbol, there is more than one potential state transition.
- further, an e-NFA includes transitions on the empty string. That is to say
  that we can transition from one state to another without consuming input.
- an illustrative example, Thompson's construction, provides an implementation
  for a regular expression
    - starting from a basis, with accepting state i, non accepting state f,
        and a transition on the empty string epsilon, we construct the NFA
        inductively.
    - we describe 3 operations, union (R = S|T), concatenation (R = S \* T), and
      closure (R = S\*)
    - these operators have precedence, namely closure, concatentation, union.
        - we want also to support operators such as +, and ? (and negation?) 
- what would be an example of a language that isn't regular? 
    - L = \{(^n)^n : n >= 0\} may have infinitely many states, since it must
      match precisely n closed parenthesis given n open, the state machine will
      grow without bound. And so it turns out that determining whether a string
      has balanced parentheses is not possible with a formal regular language
- modeling a regex:
a(b \* | c) | a
-> {., 1, ()}
-> {., 1, (a)}
-> {., 1, (a)}, {., 1, ()}
-> {., 1, (a)}, {., 1, (b)}
-> {., 1, (a)}, {., \*, (b)}
-> {., 1, (a)}, {|, \*, (b)}, {., 1, ()}
-> {., 1, (a)}, {|, \*, (b)}, {., 1, (c)}
-> {., 1, (a)}, {|, \*, (b, {., 1, (c)})}
-> {., 1, (a, {|, \*, (b, {., 1, (c)})})}
-> {., 1, (a)}, {|, ({|, \*, (b, {., 1, (c)})})}
-> {., 1, (a)}, {|, ({|, \*, (b, {., 1, (c)})}, a)}
-> {., 1, (a)}, {|, ({|, \*, (b)}, c, a)}
-> {., 1, (a, {|, ({|, \*, (b)}, c, a)})}

## Modern C Library Design
- "value oriented design" -- PASS BY VALUE, for example
```c
typedef struct vec2 { float x,y; } vec2;
vec2 vec2add(vec2 a, vec2 b) {
    vec2 result = { a.x + b.x, a.y + b.y };
    return result;
}
vec2 v = vec2add(a, (vec2) { ...  });
``` 
- counterintuitively, performance can be significantly better with this
approach
- additionally, we can create unions of nameless structs to help with reuse of
  code, allowing "names" to fit better the context
```c
typedef union hmm_vec2 {
    struct { float X, Y; };
    struct { float U, V; };
    struct { float Left, Right; };
    struct { float Width, Height; };
    float Elements[2];
} hmm_vec2;
```
- what about error handling? the modern approach is to simply store an
  additional member, essentially a boolean value, indicating whether some
  initialization was successful. this is checked at the call site, and then
  additional information is communicated in a centralized manner, such as
  through a logger.
    - we only need to check if the final operation is valid! in all of the
      function calls, we check for valid, so that this parameter is propagated.
```c
file_contents_t fc = read_file_contents("milo.cat");
image_t img = load_image_from_file_contents(fc);
texture_t texture = load_texture_from_image(img);

if (texture.valid) {
    // ...
}
```
- an example of relying on users providing their own allocators, and using
 temporary allocators to implement a sort of fast and light garbage collection
```c
allocator_t temp_allocator = make_allocator(arena);

while(game_is_running) {
    ...
    dynarr(string) strings = get_strings(temp_allocator);
    ...
    free_temp_allocator(temp_allocator);
}
```
- strlen trashes your cache. avoid. avoid libc in general, but in particular
  definitely avoid the string functions

package server

import "context"

type Relay struct {
	servers []*Server
}

func NewRelay(ctx context.Context, config Config) *Relay {
	relay := &Relay{
		servers: make([]*Server, len(config.Servers)),
	}

	for i, serverConfig := range config.Servers {
		server := NewServer(ctx, serverConfig)
		relay.servers[i] = server
	}

	return relay
}

func (relay *Relay) Close() {
	for _, server := range relay.servers {
		server.Close()
	}
}

func (relay *Relay) Run() {
	for _, server := range relay.servers {
		server.Run()
	}
}
